#pragma once
//#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>
#include "gmock/gmock.h"
#include "TcpConnection.h"

class MockTcpConnection : public TcpConnection {
public:
    MOCK_METHOD(void, Start, (), (override));
protected:
    MOCK_METHOD(void, handleWrite, (const boost::system::error_code& error, size_t bytes_transferred), (override));
    MOCK_METHOD(void, handleRead, (const boost::system::error_code& error, size_t bytes_transferred), (override));
};
