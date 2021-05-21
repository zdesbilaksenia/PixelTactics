#pragma once

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"


class GameTcpClient : public TcpClient<GameMsgTypes> {
public:
    void getHeroesStats();
    void joinLobby(uint16_t lobbyID);
    void makeLobby();
    void pingServer();
    void messageAll();
};

