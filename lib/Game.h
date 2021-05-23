#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include "../lib/pole.h"
#include "../lib/player.h"
#include "../DataBase/MySQL.h"
#include "PowerMapper.h"
#include "request.h"
#include "Lobby.h"


class Game {
public:
    
    Game(Lobby* lobby_):lobby(lobby_){}
    void StartGame();
    void CallPechkin(int playerId, const Message<GameMsgTypes> &msg);
    
    void LobbyShortCoordinates(int& line, int& cell){
        lobby->incoming().wait();
        auto msg = lobby->incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::GameShortCoordinates) {
            msg >> line >> cell;
        } else {
            std::cout << "Error in short Coordinates!\n";
            return;
        }
    }
    
private:
    Lobby* lobby;
};

#endif