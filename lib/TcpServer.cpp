#include "TcpServer.h"

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "Message.h"
#include "OwnedMessage.h"
#include "TsQueue.h"
#include "GameMsgTypes.h"


template <typename T>
TcpServer<T>::TcpServer(std::string ip, uint16_t port)
        : acceptor(ioContext, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(ip), port)) {
    }
template <typename T>
TcpServer<T>::~TcpServer() { stop(); }

template <typename T>
bool TcpServer<T>::start() {
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
template <typename T>
void TcpServer<T>::stop() {
    ioContext.stop();
    if (thrContext.joinable())
        thrContext.join();

    std::cout << "[SERVER] Stopped!";
}

template <typename T>
void TcpServer<T>::waitForConnection() {
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
                    deqConnections.back()->connectToClient(this, IDCounter++);
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
template <typename T>
void TcpServer<T>::messageToClient(boost::shared_ptr<TcpConnection<T>> client, const Message<T>& msg) {
    if (client && client->isConnected()) {
        client->send(msg);
    } else {
        onClientDisconnect(client);
        client.reset();
        deqConnections.erase(std::remove(deqConnections.begin(), deqConnections.end(), client), deqConnections.end());
    }
}
template <typename T>
void TcpServer<T>::messageAllClients(const Message<T>& msg, boost::shared_ptr<TcpConnection<T>> ignoredClient) {
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

template <typename T>
void TcpServer<T>::update(size_t maxMsgs, bool wait) {
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

template <typename T>
 void TcpServer<T>::onClientValidated(boost::shared_ptr<TcpConnection<T>> client) {
}
template <typename T>
 bool TcpServer<T>::onClientConnect(boost::shared_ptr<TcpConnection<T>> client) { return false; };

template <typename T>
 void TcpServer<T>::onClientDisconnect(boost::shared_ptr<TcpConnection<T>> client) {
}
template <typename T>
 void TcpServer<T>::onMessage(boost::shared_ptr<TcpConnection<T>> client, Message<T>& msg) {
}

template class TcpServer<GameMsgTypes>;
