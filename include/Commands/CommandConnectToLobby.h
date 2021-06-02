#include "Command.h"
#include "GameTcpClient.h"
#include "GameMsgTypes.h"

#include <boost/log/trivial.hpp>
using namespace boost::log;

class CommandConnectToLobby : public Command
{
public:
    CommandConnectToLobby(GameTcpClient& _client, bool& _lobbyWasCreated);
    void execute() override;

    ~CommandConnectToLobby();

private:
    bool& lobbyWasCreated;
    GameTcpClient& client;
};