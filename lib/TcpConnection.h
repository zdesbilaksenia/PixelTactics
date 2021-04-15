#pragma once
//#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class TcpConnection : public boost::enable_shared_from_this<TcpConnection> {
public:
    typedef boost::shared_ptr<TcpConnection> pointer;

    //virtual ~TcpConnection() = 0;
    virtual ~TcpConnection();
    static pointer Create(boost::asio::io_context& io_context);
    boost::asio::ip::tcp::socket& Socket();
    virtual void Start() = 0;

protected:
    TcpConnection(boost::asio::io_context& _ioContext);

    virtual void handleWrite(const boost::system::error_code& error, size_t bytes_transferred) = 0;

    virtual void handleRead(const boost::system::error_code& error, size_t bytes_transferred) = 0;

    boost::asio::ip::tcp::socket socket;
    char message[128];
};
