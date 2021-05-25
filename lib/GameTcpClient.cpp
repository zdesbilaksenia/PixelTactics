#include "GameTcpClient.h"

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

GameTcpClient::GameTcpClient() {}

void GameTcpClient::getHeroesStats() const {
    Message<GameMsgTypes> msg(GameMsgTypes::GetHeroesStats);
    send(msg);
}
void GameTcpClient::connectToLobby() const {
    Message<GameMsgTypes> msg(GameMsgTypes::ConnectToLobby);
    send(msg);
}
void GameTcpClient::pingServer() const {
    Message<GameMsgTypes> msg(GameMsgTypes::ServerPing);
    msg << 50;
    send(msg);
}
void GameTcpClient::messageAll() const {
    Message<GameMsgTypes> msg(GameMsgTypes::MessageAll);
    send(msg);
}

