#include "GameTcpClient.h"

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

GameTcpClient::GameTcpClient() {}

void GameTcpClient::connectToLobby() const {
    Message<GameMsgTypes> msg(GameMsgTypes::ConnectToLobby);
    send(msg);
}

