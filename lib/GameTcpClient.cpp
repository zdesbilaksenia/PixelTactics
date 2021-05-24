#include "GameTcpClient.h"

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

#include "iostream"
using namespace std;

GameTcpClient::GameTcpClient() : side(0) {}

void GameTcpClient::getHeroesStats() const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GetHeroesStats);
    send(msg);
}
void GameTcpClient::joinLobby(uint16_t lobbyID) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::JoinLobby);
    msg << lobbyID;
    send(msg);
}
void GameTcpClient::makeLobby() const
{
    Message<GameMsgTypes> msg(GameMsgTypes::MakeLobby);
    send(msg);
}
void GameTcpClient::pingServer() const
{
    Message<GameMsgTypes> msg(GameMsgTypes::ServerPing);
    msg << 50;
    send(msg);
}
void GameTcpClient::messageAll() const
{
    Message<GameMsgTypes> msg(GameMsgTypes::MessageAll);
    send(msg);
}
void GameTcpClient::sendCardReleased(const int unitId, const int posX, const int posY) const
{
    cout << "side = " << side << endl;
    Message<GameMsgTypes> msg(GameMsgTypes::GameCardFromHandChoice);
    msg << side << posY << posX << unitId;
    send(msg);
}

//По сути, isSecond
void GameTcpClient::setSide(bool isFirst)
{
    side = isFirst;
}