// #include <boost/asio.hpp>

#include "GameTcpServer.h"
#include "GameMsgTypes.h"

int main()
{
    GameTcpServer s("10.147.17.33");
    s.start();
    while(1) {
        s.update(-1, true);
    }
    return 0;
}
