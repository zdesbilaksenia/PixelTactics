#include "Commands/CommandMakeLobby.h"
#include <iostream>
using namespace std;

CommandMakeLobby::CommandMakeLobby(GameTcpClient *_client, bool &_lobbyWasCreated) : lobbyWasCreated(_lobbyWasCreated), client(_client) {}

void CommandMakeLobby::execute()
{
    if (client->isConnected())
    {
        BOOST_LOG_TRIVIAL(info) << "CommandMakeLobby::execute() : client is connected!!";
        client->makeLobby();
        client->incoming().wait();
        auto msg = client->incoming().popFront().msg;
        cout << msg << endl;
        if (msg.header.id == GameMsgTypes::LobbyWaitingForPlayer)
        {
            BOOST_LOG_TRIVIAL(info) << "CommandMakeLobby::execute() : Lobby waiting for player";
            client->incoming().wait();
            msg = client->incoming().popFront().msg;
            if (msg.header.id == GameMsgTypes::LobbyGameStart)
            {
                lobbyWasCreated = true;
                cout << "CommandMakeLobby::execute() : lobbyWasCreated = " << lobbyWasCreated << endl;
                BOOST_LOG_TRIVIAL(info) << "CommandMakeLobby::execute() : Lobby Game started!";
                return;
            }
        }
        else
        {
            BOOST_LOG_TRIVIAL(error) << "CommandMakeLobby::execute() : Didn't recieve LobbyWaitingForPlayer";
        }
    }
}

CommandMakeLobby::~CommandMakeLobby()
{
    client = nullptr;
}