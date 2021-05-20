#include "Lobby.h"

#include <vector>
#include <cstdint>
#include <boost/shared_ptr.hpp>

#include "TsQueue.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Message.h"
#include "GameMsgTypes.h"


Lobby::Lobby(uint8_t lobbyID_,TsQueue<OwnedMessage<GameMsgTypes>>* pQMsgServer_, TcpServer<GameMsgTypes>* server_) : lobbyID(lobbyID_), pQMsgServer(pQMsgServer_), server(server_) {}
Lobby::Lobby(const Lobby& other) : lobbyID(other.lobbyID), pQMsgServer(other.pQMsgServer), qMsgIn(other.qMsgIn), server(other.server) {
    for (size_t i = 0; i < other.lobby.size(); ++i) {
        if (other.lobby[i]) {
            lobby.push_back((other.lobby[i])->shared_from_this());
        }
    }
}
Lobby& Lobby::operator=(const Lobby& other) {
    if (lobbyID != other.lobbyID) {
        lobbyID = other.lobbyID;
        pQMsgServer = other.pQMsgServer;
        server = other.server;
        clearLobby();
        for (size_t i = 0; i < other.lobby.size(); ++i) {
            if (other.lobby[i]) {
                lobby.push_back((other.lobby[i])->shared_from_this());
            }
        }
    }
    return *this;
}

Lobby::~Lobby() { clearLobby(); }

void Lobby::gameStart() {
    msgLobbyGameStart();
}
void Lobby::gameOver() {
    msgLobbyGameOver();
    clearLobby();
}
void Lobby::addPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> connection) {
    if (lobby.size() == 2) {
        return;
    }
    if (!connection) {
        return;
    }
    lobby.push_back(std::move(connection));
    if (lobby.size() == 2) {
        if (playersOnline()) {
            std::cout << "[LOBBY#" << lobbyID << "]: Lobby Full!\n";
            msgLobbyFull();
            gameStart();
            return;
        }
    }
    msgLobbyWaitingForPlayer();
}

void Lobby::messageToServer(const Message<GameMsgTypes>& msg) {
    if (pQMsgServer) {
        pQMsgServer->pushBack({ nullptr, msg });
    }
}
void Lobby::messageToPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> player, const Message<GameMsgTypes>& msg) {
    if (player && player->isConnected()) {
        server->messageToClient(player, msg);
    } else {
        player.reset();
        lobby.erase(std::remove(lobby.begin(), lobby.end(), player), lobby.end());
        onPlayerDisconnected();
    }
}
void Lobby::messageAllPlayers(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> ignoredPlayer) {
    bool invalidPlayerExists = false;
    for (auto it = lobby.begin(); it != lobby.end();) {
        if ((*it)->isConnected()) {
            if ((*it) != ignoredPlayer)
                server->messageToClient((*it), msg);
            ++it;
        } else {
            (*it).reset();
            it = lobby.erase(it);
        }
    }
    if (invalidPlayerExists) {
        onPlayerDisconnected();
    }
}

void Lobby::msgLobbyWaitingForPlayer() {
    Message<GameMsgTypes> msg(GameMsgTypes::LobbyWaitingForPlayer);
    messageAllPlayers(msg);
}
void Lobby::msgLobbyFull() {
    Message<GameMsgTypes> msg(GameMsgTypes::LobbyFull);
    msg << lobbyID;
    messageToServer(msg);

}
void Lobby::msgLobbyGameStart() {
    Message<GameMsgTypes> msg(GameMsgTypes::LobbyGameStart);
    messageAllPlayers(msg);
}
void Lobby::msgLobbyGameOver() {
    Message<GameMsgTypes> msg;
    msg.header.id = GameMsgTypes::LobbyGameOver;
    msg << lobbyID;
    messageToServer(msg);
    messageAllPlayers(msg);
}
bool Lobby::playersOnline() {
    bool invalidPlayerExists = false;
    for (auto it = lobby.begin(); it != lobby.end();) {
        if ((*it) && (*it)->isConnected()) {
            ++it;
        } else {
            (*it).reset();
            it = lobby.erase(it);
            invalidPlayerExists = true;
        }
    }
    return !invalidPlayerExists;
}
void Lobby::onPlayerDisconnected() {
    Message<GameMsgTypes> msg(GameMsgTypes::LobbyPlayerDisconnected);
    messageAllPlayers(msg);
    gameOver();
}
void Lobby::clearLobby() {
    for (auto it = lobby.begin(); it != lobby.end();) {
        (*it).reset();
        it = lobby.erase(it);
    }
}
