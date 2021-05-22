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
    
private:
    Lobby* lobby;
};

#endif