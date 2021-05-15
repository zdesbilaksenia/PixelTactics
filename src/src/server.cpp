#include <chrono>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "../include/TcpConnection.h"

enum class CustomMsgTypes : uint32_t {
    ServerAccept,
    ServerDeny,
    ServerPing,
    MessageAll,
    ServerMessage
};

template <typename T>
struct MessageHeader {
    T id{};
    uint32_t size = 0;
};

template <typename T>
struct Message {
    MessageHeader<T> header{};
    std::vector<uint8_t> body;

    size_t size() const {
        // return sizeof(MessageHeader<T>) + body.size();
        return body.size();
    }

    friend std::ostream& operator << (std::ostream& os, const Message<T>& msg) {
        os << "ID:" << int(msg.header.id) << " Size:" << msg.header.size;
        return os;
    }

    template <typename DT>
    friend Message<T>& operator << (Message<T>& msg, const DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data is too complex to push into vector");

        size_t cache = msg.body.size();
        msg.body.resize(msg.body.size() + sizeof(DT));
        std::memcpy(msg.body.data() + cache, &data, sizeof(DT));
        msg.header.size = msg.size();

        return msg;
    }

    template <typename DT>
    friend Message<T>& operator >> (Message<T>& msg, DT& data) {
        static_assert(std::is_standard_layout<DT>::value, "Data is too complex to push into vector");

        size_t cache = msg.body.size() - sizeof(DT);
        std::memcpy(&data, msg.body.data() + cache, sizeof(DT));
        msg.body.resize(cache);
        msg.header.size = msg.size();

        return msg;
    }
};

template <typename T>
class TsQueue {
public:
    TsQueue() = default;
    TsQueue(const TsQueue<T>&) = delete;
    TsQueue<T>& operator = (const TsQueue<T>&) = delete;
    virtual ~TsQueue() { clear(); };

    const T& front() {
        std::scoped_lock lock(mutex);
        return deq.front();
    }

    const T& back() {
        std::scoped_lock lock(mutex);
        return deq.back();
    }

    void pushFront(const T& item) {
        std::scoped_lock lock(mutex);
        deq.emplace_front(item);

        std::unique_lock<std::mutex> ul(mutexBlock);
        cvBlock.notify_one();
    }

    void pushBack(const T& item) {
        std::scoped_lock lock(mutex);
        deq.emplace_back(item);

        std::unique_lock<std::mutex> ul(mutexBlock);
        cvBlock.notify_one();
    }

    bool empty() {
        std::scoped_lock lock(mutex);
        return deq.empty();
    }

    size_t count() {
        std::scoped_lock lock(mutex);
        return deq.size();
    }

    void clear() {
        std::scoped_lock lock(mutex);
        deq.clear();
    }

    T popFront() {
        std::scoped_lock lock(mutex);
        T cache = std::move(deq.front());
        deq.pop_front();
        return cache;
    }

    T popBack() {
        std::scoped_lock lock(mutex);
        T cache = std::move(deq.back());
        deq.pop_back();
        return cache;
    }

    void wait() {
        while (empty()) {
            std::unique_lock<std::mutex> ul(mutexBlock);
            cvBlock.wait(ul);
        }
    }

protected:
    std::mutex mutex;
    std::deque<T> deq;

    std::condition_variable cvBlock;
    std::mutex mutexBlock;
};

template <typename T>
class TcpConnection;

template <typename T>
struct OwnedMessage {
    boost::shared_ptr<TcpConnection<T>> remote = nullptr;
    Message<T> msg;

    friend std::ostream& operator << (std::ostream& os, const OwnedMessage<T>& msg) {
        os << msg.msg;
        return os;
    }
};

template <typename T>
class TcpServer {
public:
    TcpServer(uint16_t port)
        : acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), port)) {
    }
    virtual ~TcpServer() { stop(); }

    bool start() {
        try {
            waitForConnection();
            thrContext = std::thread([this]() { ioContext.run(); });
        } catch (std::exception& e) {
            std::cerr << "[SERVER] Exception: " << e.what() << "\n";
            return false;
        }

        std::cout << "[SERVER] Started!\n";
        return true;
    }
    void stop() {
        ioContext.stop();
        if (thrContext.joinable())
            thrContext.join();

        std::cout << "[SERVER] Stopped!";
    }

    void waitForConnection() {
        boost::shared_ptr<TcpConnection<T>> newConnection;
        acceptor.async_accept(
            [this](const boost::system::error_code& ec, boost::asio::ip::tcp::socket sock) {
                if (!ec) {
                    std::cout << "[SERVER] New Connection: " << sock.remote_endpoint() << "\n";
                    boost::shared_ptr<TcpConnection<T>> newConnection =
                        boost::make_shared<TcpConnection<T>>(TcpConnection<T>::owner::server,
                                                             ioContext,
                                                             std::move(sock),
                                                             qMsgIn);
                    if (onClientConnect(newConnection)) {
                        deqConnections.push_back(std::move(newConnection));
                        deqConnections.back()->connectToClient(IDCounter++);
                        std::cout << "[" << deqConnections.back()->getID()  << "] Connection Approved!\n";
                    } else {
                        std::cout << "[-----] Connection Denied\n";
                    }
                } else {
                    std::cout << "[SERVER] New Connection Error: " << ec.message() << "\n";
                }

                waitForConnection();
            });
    }
    void messageToClient(boost::shared_ptr<TcpConnection<T>> client, const Message<T>& msg) {
        if (client && client->isConnected()) {
            client.send(msg);
        } else {
            onClientDisconnect();
            client.reset();
            deqConnections.erase(std::remove(deqConnections.begin(), deqConnections.end(), client), deqConnections.end());
        }
    }
    void messageAllClients(const Message<T>& msg, boost::shared_ptr<TcpConnection<T>> ignoredClient = nullptr) {
        bool invalidClientExists = false;

        for (auto& client : deqConnections) {
            if (client && client->isConnected()) {
                if (client != ignoredClient)
                    client->send(msg);
            } else {
                onClientDisconnect(client);
                client.reset();

                invalidClientExists = true;
            }
        }

        if (invalidClientExists) {
            deqConnections.erase(std::remove(deqConnections.begin(), deqConnections.end(), nullptr), deqConnections.end());
        }
    }

    void update(size_t maxMsgs = -1, bool wait = false) {
        if (wait) {
            qMsgIn.wait();
        }

        size_t msgCount = 0;
        while (msgCount < maxMsgs && !qMsgIn.empty()) {
            auto msg = qMsgIn.popFront();
            onMessage(msg.remote, msg.msg);
            ++msgCount;
        }
    }
protected:
    virtual bool onClientConnect(boost::shared_ptr<TcpConnection<T>> client) { return false; };

    virtual void onClientDisconnect(boost::shared_ptr<TcpConnection<T>> client) {
    }
    virtual void onMessage(boost::shared_ptr<TcpConnection<T>> client, const Message<T>& msg) {
    }

    TsQueue<OwnedMessage<T>> qMsgIn;
    std::deque<boost::shared_ptr<TcpConnection<T>>> deqConnections;
    boost::asio::io_context ioContext;
    std::thread thrContext;
    boost::asio::ip::tcp::acceptor acceptor;
    uint32_t IDCounter = 10000;
private:

};

class CustomTcpServer : public TcpServer<CustomMsgTypes> {
public:
    CustomTcpServer(uint16_t port_) : TcpServer<CustomMsgTypes>(port_)  {}
protected:
    virtual bool onClientConnect(boost::shared_ptr<TcpConnection<CustomMsgTypes>> client) {
        Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerAccept;
        client->send(msg);
        return true;
    }
    virtual void onClientDisconnect(boost::shared_ptr<TcpConnection<CustomMsgTypes>> client) {
        std::cout << "Removing client [" << client->getID() << "]\n";
    }
    virtual void onMessage(boost::shared_ptr<TcpConnection<CustomMsgTypes>> client, const Message<CustomMsgTypes>& msg) {
        switch (msg.header.id) {
            case CustomMsgTypes::ServerPing:
                std::cout << "[" << client->getID() << "]: Server Ping\n";
                client->send(msg);
                break;
            case CustomMsgTypes::MessageAll:
                std::cout << "[" << client->getID() << "]: Message All\n";
                Message<CustomMsgTypes> msg;
                msg.header.id = CustomMsgTypes::ServerMessage;
                msg << client->getID();
                messageAllClients(msg, client);
        }
    }
};

int main()
{
    CustomTcpServer s(60000);
    s.start();
    while(1) {
        s.update(-1, true);
    }
    return 0;
}
