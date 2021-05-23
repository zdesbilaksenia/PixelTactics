#include "Command.h"
#include "GameTcpClient.h"
#include "GameMsgTypes.h"

#include <boost/log/trivial.hpp>
using namespace boost::log;

class CommandMakeLobby : public Command
{
public:
    CommandMakeLobby(GameTcpClient *_client, bool &_lobbyWasCreated);
    void execute() override;

    ~CommandMakeLobby();

private:
    bool& lobbyWasCreated;
    GameTcpClient *client;
};