#pragma once

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"


class GameTcpClient : public TcpClient<GameMsgTypes> {
public:
    GameTcpClient();
    void connectToLobby() const;

    void setLobbyID(uint16_t lobbyID_);
};

