#include "GameTcpClient.h"

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

void GameTcpClient::getHeroesStats() {
    Message<GameMsgTypes> msg(GameMsgTypes::GetHeroesStats);
    send(msg);
}
void GameTcpClient::joinLobby(uint16_t lobbyID) {
    Message<GameMsgTypes> msg(GameMsgTypes::JoinLobby);
    msg << lobbyID;
    send(msg);
}
void GameTcpClient::makeLobby() {
    Message<GameMsgTypes> msg(GameMsgTypes::MakeLobby);
    send(msg);
}
void GameTcpClient::pingServer() {
    Message<GameMsgTypes> msg(GameMsgTypes::ServerPing);
    msg << 50;
    send(msg);
}
void GameTcpClient::messageAll() {
    Message<GameMsgTypes> msg(GameMsgTypes::MessageAll);
    send(msg);
}

