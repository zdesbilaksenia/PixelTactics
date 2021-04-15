#pragma once
//#include <boost/bind/bind.hpp>
//#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

class ServerTcpConnection : public TcpConnection {
public:
    ~ServerTcpConnection();
    void Start() override;

protected:
    void handleWrite(const boost::system::error_code& error, size_t bytes_transferred) override;
    void handleRead(const boost::system::error_code& error, size_t bytes_transferred) override;
};
