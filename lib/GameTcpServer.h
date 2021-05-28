#pragma once

#include <boost/shared_ptr.hpp>
#include <cstdint>
#include <map>

#include "TcpConnection.h"
#include "Message.h"
#include "TcpServer.h"
#include "Lobby.h"
#include "GameMsgTypes.h"

class GameTcpServer : public TcpServer<GameMsgTypes> {
public:
    GameTcpServer(std::string ip = "127.0.0.1", uint16_t port = 55555, uint8_t maxLobbiesNum_ = 5);

    void killLobby(uint16_t lobbyID);
protected:
    virtual void onClientValidated(boost::shared_ptr<TcpConnection<GameMsgTypes>> client);
    virtual bool onClientConnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client);
    virtual void onClientDisconnect(boost::shared_ptr<TcpConnection<GameMsgTypes>> client);
    virtual void onMessage(boost::shared_ptr<TcpConnection<GameMsgTypes>> client, Message<GameMsgTypes>& msg);
private:
    void messageToLobby(Message<GameMsgTypes> msg, boost::shared_ptr<TcpConnection<GameMsgTypes>> client = nullptr);

    std::map<uint16_t, Lobby> lobbies;
    std::map<boost::shared_ptr<TcpConnection<GameMsgTypes>>, uint16_t> players;
    uint16_t maxLobbiesNum;
    uint16_t lobbyIDs;
};

