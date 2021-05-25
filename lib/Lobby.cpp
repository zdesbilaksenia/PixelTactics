#include "Lobby.h"

#include <vector>
#include <cstdint>
#include <boost/shared_ptr.hpp>

#include "TsQueue.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Message.h"
#include "GameMsgTypes.h"
#include "Game.h"


Lobby::Lobby(uint8_t lobbyID_,TsQueue<OwnedMessage<GameMsgTypes>>* pQMsgServer_, TcpServer<GameMsgTypes>* server_)
    : lobbyID(lobbyID_), statusType(status::incomplete), pQMsgServer(pQMsgServer_), server(server_) {}
Lobby::Lobby(const Lobby& other)
    : lobbyID(other.lobbyID), statusType(other.statusType), pQMsgServer(other.pQMsgServer), qMsgIn(other.qMsgIn), server(other.server) {
    for (size_t i = 0; i < other.lobby.size(); ++i) {
        if (other.lobby[i]) {
            lobby.push_back((other.lobby[i])->shared_from_this());
        }
    }
}
Lobby& Lobby::operator=(const Lobby& other) {
    if (lobbyID != other.lobbyID) {
        lobbyID = other.lobbyID;
        statusType = other.statusType;
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

Lobby::~Lobby() {
    clearLobby();
    if (gameThr.joinable()) {
        gameThr.join();
    }
}

TsQueue<OwnedMessage<GameMsgTypes>>& Lobby::incoming() {
    return qMsgIn;
}


void Lobby::gameStart() {
    msgLobbyGameStart();
    gameThr = std::thread(
        [this]() {
            Game game(this);
            game.StartGame();
    });
}

 void Lobby::sendToPlayer(int playerId, const Message<GameMsgTypes>& msg) {
    messageToPlayer(lobby[playerId]->shared_from_this(), msg);
}

void Lobby::gameOver() {
    msgLobbyGameOver();
}
void Lobby::addPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> connection) {
    if (lobby.size() == 2 || !connection) {
        return;
    }
    lobby.push_back(std::move(connection));
    if (lobby.size() == 2) {
        statusType = status::full;
        msgLobbyFull();
        gameStart();
        return;
    }
    msgLobbyWaitingForPlayer();
}

bool Lobby::isFull() const {
    return statusType == status::full;
}

void Lobby::addMessage(const OwnedMessage<GameMsgTypes>& msg) {
    if (msg.msg.header.id == GameMsgTypes::LobbyLeave) {
        onPlayerDisconnected();
    }
    qMsgIn.pushBack(msg);
}

void Lobby::messageToServer(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> player) {
    if (pQMsgServer) {
        pQMsgServer->pushBack({ std::move(player), msg });
    }
}
void Lobby::messageToPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> player, const Message<GameMsgTypes>& msg) {
    if (server) {
        server->messageToClient(std::move(player), msg);
    }
}
void Lobby::messageAllPlayers(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> ignoredPlayer) {
    bool invalidPlayerExists = false;
    std::cout << "lobby size: " << lobby.size() << std::endl;
    for (auto it = lobby.begin(); it != lobby.end();it++) {
        if ((*it) && (*it)->isConnected() && (*it) != ignoredPlayer) {
            server->messageToClient((*it)->shared_from_this(), msg);
        }
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

    Message<GameMsgTypes> msg(GameMsgTypes::LobbyGameOver);
    msg << lobbyID;
    messageToServer(msg);

}

void Lobby::onPlayerDisconnected() {
    if (statusType == status::incomplete) {
        Message<GameMsgTypes> msg(GameMsgTypes::LobbyKill);
        msg << lobbyID;
        messageToServer(msg);
    }
    // if lobby is full (so game is running), game logic will see the
    // message about disconnected player and finish itself
}
void Lobby::clearLobby() {
    for (auto it = lobby.begin(); it != lobby.end();) {
        (*it).reset();
        it = lobby.erase(it);
    }
}
