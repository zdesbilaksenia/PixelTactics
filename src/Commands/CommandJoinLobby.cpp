#include "Commands/CommandJoinLobby.h"

CommandJoinLobby::CommandJoinLobby(GameTcpClient &_client, bool &_joinedLobby) : client(_client), joinedLobby(_joinedLobby) {}

void CommandJoinLobby::execute()
{
    client.joinLobby(1);
    if (client.isConnected())
    {
        BOOST_LOG_TRIVIAL(info) << "CommandJoinLobby::execute() : Lobby waiting for player";
        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::LobbyGameStart)
        {
            BOOST_LOG_TRIVIAL(info) << "CommandJoinLobby::execute() : Lobby Game started!";
            joinedLobby = true;
            return;
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "CommandJoinLobby::execute() : Didn't recieve LobbyWaitingForPlayer";
        }
    }
}

CommandJoinLobby::~CommandJoinLobby() {}