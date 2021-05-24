#include "Command.h"
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
#include <boost/log/trivial.hpp>
using namespace boost::log;

class CommandJoinLobby : public Command
{
public:
    CommandJoinLobby(GameTcpClient &_client, bool &_joinedLobby);

    void execute() override;

    ~CommandJoinLobby();

private:
    bool &joinedLobby;
    GameTcpClient &client;
};
