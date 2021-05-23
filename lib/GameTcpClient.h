#pragma once

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"


class GameTcpClient : public TcpClient<GameMsgTypes> {
public:
    GameTcpClient();
    void getHeroesStats() const;
    void joinLobby(uint16_t lobbyID) const;
    void makeLobby() const;
    void pingServer() const;
    void messageAll() const;

    void setLobbyID(uint16_t lobbyID_);
};

