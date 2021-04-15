#pragma once
//#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"

class TcpServer {
public:
    TcpServer(boost::asio::io_context& _ioContext);
    TcpServer();
    virtual ~TcpServer() = 0;

protected:
    virtual void startAccept() = 0;
    virtual void handleAccept(TcpConnection::pointer newConnection, const boost::system::error_code& error) = 0;

    boost::asio::io_context ioContext;
    boost::asio::ip::tcp::acceptor acceptor;
};
