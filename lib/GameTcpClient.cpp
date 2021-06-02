#include "GameTcpClient.h"

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

#include <iostream>
using std::cout;
using std::endl;

GameTcpClient::GameTcpClient() : side(0) {}

void GameTcpClient::connectToLobby() const {
    Message<GameMsgTypes> msg(GameMsgTypes::ConnectToLobby);
    send(msg);
}

void GameTcpClient::sendCardReleased(const int unitId, const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameCardFromHandChoice);
    msg << side << posY << posX << unitId;
    send(msg);
}

void GameTcpClient::sendAttackerPos(const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameAttackRequest);
    msg << posY << posX;
    send(msg);
}

void GameTcpClient::sendPowerUserPos(const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GamePowerRequest);
    msg << posY << posX;
    send(msg);
}

void GameTcpClient::sendAttackedPos(const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameAttackRequest);
    msg << posY << posX;
    send(msg);
}

void GameTcpClient::sendPowerTargetPos(const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GamePowerRequest);
    msg << posY << posX;
    send(msg);
}

void GameTcpClient::sendTakeCard()
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameTakeCard);
    send(msg);
}

//По сути, isSecond
void GameTcpClient::setSide(bool isFirst)
{
    side = isFirst;
}

void GameTcpClient::sendRemoveBodyRequest()
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameRemoveBodyRequest);
    send(msg);
    cout << "msg sent" << endl;
}

void GameTcpClient::sendRemovedBody(const int posX, const int posY) const
{
    Message<GameMsgTypes> msg(GameMsgTypes::GameRemoveBody);
    msg << posY << posX;
    send(msg);
}