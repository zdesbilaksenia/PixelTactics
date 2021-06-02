#pragma once

#include <boost/shared_ptr.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "Message.h"
#include "OwnedMessage.h"
#include "TsQueue.h"

// template <typename T>
// class TcpConnection;

template <typename T>
class TcpServer {
public:
    TcpServer(std::string ip = "127.0.0.1", uint16_t port = 55555);
    virtual ~TcpServer();
    bool start();
    void stop();

    virtual void waitForConnection();

    void removeDisconnectedClients();

    void messageToClient(boost::shared_ptr<TcpConnection<T>> client, const Message<T>& msg);

    void messageAllClients(const Message<T>& msg, boost::shared_ptr<TcpConnection<T>> ignoredClient = nullptr);

    void update(size_t maxMsgs = -1, bool wait = false);

    virtual void onClientValidated(boost::shared_ptr<TcpConnection<T>> client);
protected:
    virtual bool onClientConnect(boost::shared_ptr<TcpConnection<T>> client);

    virtual void onClientDisconnect(boost::shared_ptr<TcpConnection<T>> client);
    virtual void onMessage(boost::shared_ptr<TcpConnection<T>> client, Message<T>& msg);

    TsQueue<OwnedMessage<T>> qMsgIn;
    std::deque<boost::shared_ptr<TcpConnection<T>>> deqConnections;
    boost::asio::io_context ioContext;
    std::thread thrContext;
    boost::asio::ip::tcp::acceptor acceptor;
    uint32_t IDCounter = 10000;
};


