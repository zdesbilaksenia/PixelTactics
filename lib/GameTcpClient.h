#pragma once

#include <cstdint>

#include "Message.h"
#include "TcpClient.h"
#include "GameMsgTypes.h"

class GameTcpClient : public TcpClient<GameMsgTypes>
{
public:
    GameTcpClient();
    void getHeroesStats() const;
    void joinLobby(uint16_t lobbyID) const;
    void makeLobby() const;
    void pingServer() const;
    void messageAll() const;
    void sendCardReleased(const int, const int, const int) const;
    void sendAttackerPos(const int, const int) const;
    void sendAttackedPos(const int, const int) const;
    void sendAttack() const;
    //.......
    //

    void setLobbyID(uint16_t lobbyID_);
    void setSide(bool isFirst);
    int getSide()
    {
        return side;
    }

private:
    //0 - первый игрок
    //1 - второй
    int side;
};
