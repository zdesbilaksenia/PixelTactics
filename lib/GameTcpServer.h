#pragma once
#include <boost/bind/bind.hpp>
//#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "ServerTcpConnection.h"

class GameTcpServer : public TcpServer {
public:
    ~GameTcpServer() override;

protected:
    void startAccept() override;
    void handleAccept(ServerTcpConnection::pointer newConnection, const boost::system::error_code& error) override;

    std::vector<boost::thread> threads;
};
