#pragma once

#include <boost/move/unique_ptr.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "Message.h"
#include "OwnedMessage.h"
#include "TsQueue.h"


template <typename T>
class TcpClient {
public:
    TcpClient();
    ~TcpClient();

    bool connect(const std::string& host = "127.0.0.1", const uint16_t port = 55555);
    void disconnect();
    bool isConnected() const;

    TsQueue<OwnedMessage<T>>& incoming();
    void send(const Message<T>& msg) const;
    bool waitForMessageOnTime(int sec);

protected:
    boost::asio::io_context ioContext;
    boost::movelib::unique_ptr<TcpConnection<T>> connection;
    std::thread thrContext;
private:
    TsQueue<OwnedMessage<T>> qMsgIn;
};

