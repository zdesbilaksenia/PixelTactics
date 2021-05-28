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


GameTcpServer::GameTcpServer(std::string ip, uint16_t port, uint8_t maxLobbiesNum_)
    : TcpServer<GameMsgTypes>(ip, port), maxLobbiesNum(maxLobbiesNum_), lobbyIDs(0) {
}

void GameTcpServer::onClientValidated(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
}

bool GameTcpServer::onClientConnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
    removeDisconnectedClients();
    if (deqConnections.size() == (maxLobbiesNum * 2)) {
        return false;
    }
    Message<GameMsgTypes> msg;
    msg.header.id = GameMsgTypes::ServerAccept;
    client->send(msg);

    std::cout << "lobbies num: " << lobbies.size() << std::endl;
    std::cout << "players num: " << players.size() << std::endl;

    return true;
}
void GameTcpServer::onClientDisconnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
    auto it = players.find(client);
    if (it != players.end()) {
        std::cout << "removinf player: " << client->getID() << std::endl;
        Message<GameMsgTypes> msg(GameMsgTypes::LobbyLeave);
        messageToLobby(msg, client);
        players.erase(it);
    }
    std::cout << "Removing client [" << client->getID() << "]\n";
}
void GameTcpServer::onMessage(boost::shared_ptr<TcpConnection<GameMsgTypes>> client, Message<GameMsgTypes>& msg) {
    switch (msg.header.id) {
        case GameMsgTypes::ConnectToLobby: {
            removeDisconnectedClients();
            if (lobbies.size() == 0 || lobbies[lobbyIDs].isFull()) {
                if (lobbies.size() == 0) {
                    std::cout << "No lobbies FOR NOW!!\n";
                }
                if (lobbies.size() == maxLobbiesNum) {
                    Message<GameMsgTypes> outMsg(GameMsgTypes::ErrorMessage);
                    std::string err = "All lobbies are busy.\n";
                    outMsg << err;
                    client->send(outMsg);
                    return;
                }
                Lobby lobby(++lobbyIDs, &qMsgIn, this);
                lobbies[lobbyIDs] = lobby;
                std::cout << "[SERVER]: Made Lobby with ID: " << lobbyIDs << std::endl;
            }
            std::cout << "[SERVER]: Adding player in lobby#" << lobbyIDs << "\n";
            players[client] = lobbyIDs;
            lobbies[lobbyIDs].addPlayer(std::move(client));
            break;
        }
        case GameMsgTypes::LobbyFull: {
            uint16_t lobbyID = 0;
            std::cout << "Trying to take lobbyID\n";
            msg >> lobbyID;
            std::cout << "[Lobby#" << lobbyID << "]: Lobby Full\n";
            break;
        }
        case GameMsgTypes::LobbyLeave: {
            messageToLobby(msg, client);
            players.erase(client);
            break;
        }
        case GameMsgTypes::LobbyGameOver: {
            uint16_t lobbyID = 0;
            msg >> lobbyID;
            std::cout << "[Lobby#" << lobbyID << "]: Game Over\n";
            killLobby(lobbyID);
            break;
        }
        default: {
            messageToLobby(msg, client);
        }
    }
}

void GameTcpServer::messageToLobby(Message<GameMsgTypes> msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> client) {
    lobbies[players[client]].addMessage({client, msg});
}

// Have to make this method, because if I add MsgType and send msg,
// lobby won't destruct immediately
void GameTcpServer::killLobby(uint16_t lobbyID) {
    std::cout << "Tryin to erase lobby w/ id=" << lobbyID << "\n";
    auto it = lobbies.find(lobbyID);
    if (it != lobbies.end()) {
        std::cout << "found lobby to erase\n";
        lobbies.erase(lobbyID);
    }
    std::cout << "[Lobby#" << lobbyID << "]: Lobby destructed\n";
}
