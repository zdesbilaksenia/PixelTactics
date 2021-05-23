#pragma once

#include <vector>
#include <cstdint>
#include <thread>
#include <boost/shared_ptr.hpp>

#include "TsQueue.h"
#include "TcpServer.h"
#include "TcpConnection.h"
#include "Message.h"
#include "OwnedMessage.h"
#include "GameMsgTypes.h"

class Lobby {
public:
    enum class status {
        empty,
        incomplete,
        full
    };
    Lobby() = default;
    Lobby(uint8_t lobbyID_,TsQueue<OwnedMessage<GameMsgTypes>>* pQMsgServer_, TcpServer<GameMsgTypes>* server_);
    Lobby(const Lobby& other);
    Lobby& operator=(const Lobby& other);

    ~Lobby();

    void gameStart();
    void gameOver();

    void addPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> connection);

    void addMessage(const OwnedMessage<GameMsgTypes>& msg);

    TsQueue<OwnedMessage<GameMsgTypes>>& incoming();
    void sendToPlayer(int playerId, const Message<GameMsgTypes>& msg);
private:
    void messageToServer(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> player = nullptr);
    void messageToPlayer(boost::shared_ptr<TcpConnection<GameMsgTypes>> player, const Message<GameMsgTypes>& msg);
    void messageAllPlayers(const Message<GameMsgTypes>& msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> ignoredPlayer = nullptr);

    void msgLobbyWaitingForPlayer();
    void msgLobbyFull();

    void msgLobbyGameStart();
    void msgLobbyGameOver();

    bool playersOnline();
    void onPlayerDisconnected();
    void clearLobby();

    uint16_t lobbyID;
    status statusType;
    TsQueue<OwnedMessage<GameMsgTypes>>* pQMsgServer; // Have to make it pointer, because map for lobbies needs default constructor
    TsQueue<OwnedMessage<GameMsgTypes>> qMsgIn;
    std::vector<boost::shared_ptr<TcpConnection<GameMsgTypes>>> lobby;

    TcpServer<GameMsgTypes>* server;
    std::thread gameThr;
};
