#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include <boost/asio.hpp>

#include "mocks/MockTcpConnection.h"
#include "mocks/MockTcpServer.h"

TEST(TcpConnection, Start) {
    boost::asio::io_context ioContext;
    //MockTcpConnection::pointer con = MockTcpConnection::Create(ioContext);
    //EXPECT_CALL((*con), Start).Times(1);
    MockTcpServer serv;
    EXPECT_CALL(serv, startAccept).Times(1);
}
