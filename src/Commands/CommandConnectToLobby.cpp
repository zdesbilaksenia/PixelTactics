#include "Commands/CommandConnectToLobby.h"
#include <iostream>
using std::cout;
using std::endl;

CommandConnectToLobby::CommandConnectToLobby(GameTcpClient &_client, bool& _lobbyWasCreated) : client(_client), lobbyWasCreated(_lobbyWasCreated) {}

void CommandConnectToLobby::execute()
{
    if (client.isConnected())
    {
        BOOST_LOG_TRIVIAL(info) << "CommandConnectToLobby::execute() : client is connected!!";
        client.connectToLobby();
        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        switch (msg.header.id)
        {
        case GameMsgTypes::LobbyWaitingForPlayer:
        {
            BOOST_LOG_TRIVIAL(info) << "CommandConnectToLobby::execute() : Lobby created, waiting for other player...";
            client.incoming().wait();
            msg = client.incoming().popFront().msg;
            if (msg.header.id == GameMsgTypes::LobbyGameStart)
            {
                lobbyWasCreated = true;
                client.setSide(0);
                BOOST_LOG_TRIVIAL(info) << "CommandConnectToLobby::execute() : Game started!";
                return;
            }
            break;
        }
        case GameMsgTypes::LobbyGameStart:
        {
            lobbyWasCreated = true;
            client.setSide(1);
            BOOST_LOG_TRIVIAL(info) << "CommandConnectToLobby::execute() : Game started!";
            return;
            break;
        }
        case GameMsgTypes::ErrorMessage:
        {
            lobbyWasCreated = false;
            break;
        }
        }
    }
}

CommandConnectToLobby::~CommandConnectToLobby()
{
}