#pragma once
//#include <boost/bind/bind.hpp>
#include <boost/shared_ptr.hpp>
//#include <boost/thread/thread.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

#include "TcpConnection.h"
#include "gmock/gmock.h"
#include "TcpServer.h"

class MockTcpServer : public TcpServer {
protected:
    MOCK_METHOD(void, startAccept, (), (override));
    MOCK_METHOD(void, handleAccept, (TcpConnection::pointer newConnection, const boost::system::error_code& error), (override));
};
