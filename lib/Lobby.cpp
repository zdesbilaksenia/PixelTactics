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
    : lobbyID(lobbyID_), statusType(status::empty), pQMsgServer(pQMsgServer_), server(server_) {}
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

Lobby::~Lobby() { clearLobby(); }

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
    std::cout << "msgd x2\n";
    clearLobby();
}
void Lobby::addPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> connection) {
    std::cout << "trying to add player in lobby#" << lobbyID << std::endl;
    if (lobby.size() == 2 || !connection) {
        return;
    }
    lobby.push_back(std::move(connection));
    if (lobby.size() == 2) {
        lobby.front() ? std::cout << "connected 1\n" : std::cout << "not connected 1\n";
        lobby.back() ? std::cout << "connected 2\n" : std::cout << "not connected 2\n";
        if (playersOnline()) {
            std::cout << "[LOBBY#" << lobbyID << "]: Lobby Full!\n";
            statusType = status::full;
            msgLobbyFull();
            gameStart();
            return;
        } else {
            std::cout << "game over\n";
            return;
            gameOver();
        }
    }
    msgLobbyWaitingForPlayer();
    lobby.front() ? std::cout << "connected 1\n" : std::cout << "not connected 1\n";

}

void Lobby::addMessage(const OwnedMessage<GameMsgTypes>& msg) {
    if (msg.msg.header.id == GameMsgTypes::LobbyPlayerDisconnected) {
        onPlayerDisconnected();
    }
    qMsgIn.pushBack(msg);
}

void Lobby::messageToServer(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> player) {
    std::cout << "in msg to serv\n";
    if (pQMsgServer) {
        std::cout << "pushedback\n";
        pQMsgServer->pushBack({ std::move(player), msg });
    }
}
void Lobby::messageToPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> player, const Message<GameMsgTypes>& msg) {
        server->messageToClient(std::move(player), msg);
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
    std::cout << "LOBBYID: " << lobbyID << std::endl;
    pQMsgServer ? std::cout << "has ptr to serv\n" : std::cout << "has not ptr to serv\n";
    std::cout << "in msgLobbyGameOver\n";
    Message<GameMsgTypes> gameOverMsg(GameMsgTypes::LobbyGameOver);
    gameOverMsg << lobbyID;
    messageAllPlayers(gameOverMsg);
    Message<GameMsgTypes> leaveLobbyMsg(GameMsgTypes::LobbyLeave);
    std::cout << "in leavelobby\n";
    for (auto it = lobby.begin(); it != lobby.end();) {
        messageToServer(leaveLobbyMsg, std::move((*it)));
    }
    std::cout << gameOverMsg;
    messageToServer(gameOverMsg);
    std::cout << "to serv msg\n";
}
bool Lobby::playersOnline() {
    bool invalidPlayerExists = false;
    for (auto it = lobby.begin(); it != lobby.end();) {
        if ((*it) && (*it)->isConnected()) {
            ++it;
            std::cout << "online\n";
        } else {
            (*it).reset();
            it = lobby.erase(it);
            invalidPlayerExists = true;
        }
    }
    switch (lobby.size()) {
        case 0:
            statusType = status::empty;
            break;
        case 1:
            statusType = status::incomplete;
            break;
        case 2:
            statusType = status::full;
            break;
    }
    return !invalidPlayerExists;
}
void Lobby::onPlayerDisconnected() {
    std::cout << "in messageall\n";
    Message<GameMsgTypes> msg(GameMsgTypes::LobbyPlayerDisconnected);
    messageAllPlayers(msg);
    std::cout << "messaged!\n";
    gameOver();
}
void Lobby::clearLobby() {
    for (auto it = lobby.begin(); it != lobby.end();) {
        (*it).reset();
        it = lobby.erase(it);
    }
}
