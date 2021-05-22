#include "GameTcpServer.h"

#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <iostream>
#include <map>

#include "TcpConnection.h"
#include "Message.h"
#include "TcpServer.h"
#include "Lobby.h"
#include "GameMsgTypes.h"


GameTcpServer::GameTcpServer(std::string ip, uint16_t port, uint8_t maxLobbiesNum_) : TcpServer<GameMsgTypes>(ip, port), maxLobbiesNum(maxLobbiesNum_) {
}

void GameTcpServer::onClientValidated(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
}

bool GameTcpServer::onClientConnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
    if (deqConnections.size() == (maxLobbiesNum * 2)) {
        return false;
    }
    Message<GameMsgTypes> msg;
    msg.header.id = GameMsgTypes::ServerAccept;
    client->send(msg);

    return true;
}
void GameTcpServer::onClientDisconnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
    std::cout << "Removing client [" << client->getID() << "]\n";
}
void GameTcpServer::onMessage(boost::shared_ptr<TcpConnection<GameMsgTypes>> client, Message<GameMsgTypes>& msg) {
    std::cout << "[SERVER]: Got message: " << msg << std::endl;
    switch (msg.header.id) {
        case GameMsgTypes::GetHeroesStats: {
            Message<GameMsgTypes> outMsg(GameMsgTypes::HeroesStats);
            std::cout << "[SERVER]: Got request for Stats from [" << client->getID() << "]\n";
            uint8_t stats[36];
            for (uint8_t i = 0; i < 36; ++i) {
                stats[i] = i;
            }
            outMsg << stats;
            client->send(outMsg);
            break;
        }
        case GameMsgTypes::MakeLobby: {
            if (lobbies.size() == maxLobbiesNum) {
                return;
            }
            Lobby lobby(lobbies.size() + 1, &qMsgIn, this);
            lobbies[lobbies.size() + 1] = lobby;
            std::cout << "[SERVER]: Made Lobby with ID: " << lobbies.size() << std::endl;
            lobbies[lobbies.size()].addPlayer(std::move(client));
            break;
        }
        case GameMsgTypes::JoinLobby: {
            uint16_t lobbyID = 0;
            std::cout << "Trying to take lobbyID\n";
            msg >> lobbyID;
            std::cout << "Successfully taken\n";
            if (lobbyID > uint16_t(lobbies.size())) {
                std::cout << "[SERVER]: Wrong Lobby ID\n";
                return;
            }
            std::cout << "[SERVER]: Adding player\n";
            lobbies[lobbyID].addPlayer(std::move(client));
            break;
        }
        case GameMsgTypes::ServerPing: {
            std::cout << "[" << client->getID() << "]: Server Ping\n";
            client->send(msg);
            break;
        }
        case GameMsgTypes::MessageAll: {
            std::cout << "[" << client->getID() << "]: Message All\n";
            Message<GameMsgTypes> msgOut;
            msgOut.header.id = GameMsgTypes::ServerMessage;
            msgOut << client->getID();
            messageAllClients(msgOut, client);
            break;
        }
        case GameMsgTypes::LobbyFull: {
            uint16_t lobbyID = 0;
            std::cout << "Trying to take lobbyID\n";
            msg >> lobbyID;
            std::cout << "[Lobby#" << lobbyID << "]: Lobby Full\n";
            break;
        }
        case GameMsgTypes::LobbyGameOver: {
            uint16_t lobbyID = 0;
            msg >> lobbyID;
            std::cout << "[Lobby#" << lobbyID << "]: Lobby Game Over\n";
            lobbies.erase(lobbyID);
            break;
        }
    }
}

