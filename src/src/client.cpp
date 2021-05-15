#include <chrono>
#include <mutex>
#include <thread>
#include <iostream>
#include <string>
#include <vector>
#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/move/unique_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/move/make_unique.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

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
    }

    void pushBack(const T& item) {
        std::scoped_lock lock(mutex);
        deq.emplace_back(item);
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

protected:
    std::mutex mutex;
    std::deque<T> deq;
};

template <typename T>
class TcpConnection;

template <typename T>
struct OwnedMessage {
    boost::shared_ptr<TcpConnection<T>> remote = nullptr;
    Message<T> msg;

    friend std::ostream& operator << (std::ostream& os, const OwnedMessage<T>& msg) {
        os << msg;
        return os;
    }
};

template <typename T>
class TcpConnection : public boost::enable_shared_from_this<TcpConnection<T>> {
public:
    enum class owner {
        server,
        client
    };
    TcpConnection(owner parent, boost::asio::io_context& ioContext_, boost::asio::ip::tcp::socket sock_, TsQueue<OwnedMessage<T>>& qMsgIn_)
    : ioContext(ioContext_), sock(std::move(sock_)), qMsgIn(qMsgIn_) {
        ownerType = parent;
    }
    virtual ~TcpConnection() {}

    uint32_t getID() const {
        return id;
    }

    void connectToClient(uint32_t uid = 0) {
        if (ownerType == owner::server) {
            if (sock.is_open()) {
                id = uid;
                readHeader();
            }
        }
    }
    void connectToServer(const boost::asio::ip::tcp::resolver::results_type& endpoints) {
        if (ownerType == owner::client) {
            boost::asio::async_connect(sock, endpoints,
                    [this](const boost::system::error_code& ec, boost::asio::ip::tcp::endpoint endpoint) {
                        if (!ec) {
                            readHeader();
                        }
                    });
        }
    }
    void disconnect() {
        if (isConnected()) {
            boost::asio::post(ioContext, [this]() { sock.close(); });
        }
    }
    bool isConnected() const {
        return sock.is_open();
    }

    void send(const Message<T>& msg) {
        boost::asio::post(ioContext,
                          [this, msg]() {
                              bool writingMsg = !qMsgOut.empty();
                              qMsgOut.pushBack(msg);
                              if (!writingMsg) {
                                  writeHeader();
                              }
                          });
    }
protected:
    boost::asio::ip::tcp::socket sock;
    boost::asio::io_context& ioContext;
    TsQueue<Message<T>> qMsgOut;
    TsQueue<OwnedMessage<T>>& qMsgIn;
    Message<T> tempMsg;

    owner ownerType = owner::server;
    uint32_t id = 0;

private:
    void readHeader() {
        boost::asio::async_read(sock, boost::asio::buffer(&tempMsg.header, sizeof(MessageHeader<T>)),
                [this](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        if (tempMsg.header.size > 0) {
                            tempMsg.body.resize(tempMsg.header.size);
                            readBody();
                        } else {
                            addToMsgQueue();
                        }
                    } else {
                        std::cout << "[" << id << "] Read Header Fail.\n";
                        sock.close();
                    }
                });
    }

    void readBody() {
        boost::asio::async_read(sock, boost::asio::buffer(tempMsg.body.data(), tempMsg.body.size()),
                [this](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        addToMsgQueue();
                    } else {
                        std::cout << "[" << id << "] Read Body Fail.\n";
                        sock.close();
                    }
                });
    }

    void writeHeader() {
        async_write(sock, boost::asio::buffer(&qMsgOut.front().header, sizeof(MessageHeader<T>)),
                [this](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        if (qMsgOut.front().body.size() > 0) {
                            writeBody();
                        } else {
                            qMsgOut.popFront();
                            if (!qMsgOut.empty()) {
                                writeHeader();
                            }
                        }
                    } else {
                        std::cout << "[" << id << "] Write Header Fail.\n";
                        sock.close();
                    }
                });
    }

    void writeBody() {
        std::cout << "WriteBody checkpoint\n";
        async_write(sock, boost::asio::buffer(qMsgOut.front().body.data(), qMsgOut.front().body.size()),
                [this](const boost::system::error_code& ec, std::size_t length) {
                    if (!ec) {
                        qMsgOut.popFront();

                        if (!qMsgOut.empty()) {
                            writeHeader();
                        }
                    } else {
                        std::cout << "[" << id << "] Write Body Fail.\n";
                        sock.close();
                    }
                });
    }

    void addToMsgQueue() {
        if (ownerType == owner::server) {
            qMsgIn.pushBack({ this->shared_from_this(), tempMsg });
        } else {
            qMsgIn.pushBack({ nullptr, tempMsg });
        }

        readHeader();
    }
};

template <typename T>
class TcpClient {
public:
    TcpClient() {}
    ~TcpClient() { disconnect(); }

    bool connect(const std::string& host, const uint16_t port) {
        try {
            boost::asio::ip::tcp::resolver resolver(ioContext);
            boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(host, std::to_string(port));
            connection = boost::movelib::make_unique<TcpConnection<T>>(
                    TcpConnection<T>::owner::client,
                    ioContext,
                    boost::asio::ip::tcp::socket(ioContext), qMsgIn);
            connection->connectToServer(endpoints);
            thrContext = std::thread([this]() { ioContext.run(); });
        } catch(std::exception& e) {
            std::cerr << "Client Exception: " << e.what() << "\n";
            return false;
        }
        return true;
    }
    void disconnect() {
        if (isConnected()) {
            connection->disconnect();
        }

        ioContext.stop();
        if (thrContext.joinable())
            thrContext.join();

        connection.release();
    }
    bool isConnected() const {
        if (connection)
            return connection->isConnected();

        return false;
    }

    TsQueue<OwnedMessage<T>>& incoming() {
        return qMsgIn;
    }
    void send(const Message<T>& msg) const {
        if (isConnected()) {
            connection->send(msg);
        }
    }

protected:
    boost::asio::io_context ioContext;
    boost::movelib::unique_ptr<TcpConnection<T>> connection;
    std::thread thrContext;
private:
    TsQueue<OwnedMessage<T>> qMsgIn;
};

class CustomTcpClient : public TcpClient<CustomMsgTypes> {
public:
    void pingServer() {
        Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::ServerPing;
        msg << 50;
        send(msg);
    }
    void messageAll() {
        Message<CustomMsgTypes> msg;
        msg.header.id = CustomMsgTypes::MessageAll;
        send(msg);
    }
};

int main(int argc, char* argv[])
{
    CustomTcpClient c;
    c.connect("127.0.0.1", 60000);
    c.pingServer();
    bool quit = false;
    while (!quit) {
        char choice = 0;
        std::cin >> choice;
        switch (choice) {
            case '1':
                c.pingServer();
                break;
            case '2':
                c.messageAll();
                break;
            case '3':
                quit = true;
                break;
            default:
                break;
        }
        if (c.isConnected()) {
            if (!c.incoming().empty()) {
                auto msg = c.incoming().popFront().msg;
                switch (msg.header.id) {
                    case CustomMsgTypes::ServerAccept:
                        std::cout << "Server Accepted Connection\n";
                        break;
                    case CustomMsgTypes::ServerPing:
                        int inc;
                        msg >> inc;
                        std::cout << inc << " times" << std::endl;
                        break;
                    case CustomMsgTypes::ServerMessage:
                        uint32_t uid;
                        msg >> uid;
                        std::cout << "Hello from [" << uid << "]\n";
                        break;
                    default:
                        break;
                }
            }
        } else {
            std::cout << "Server Down\n";
            quit = true;
        }
    }

    return 0;
}
