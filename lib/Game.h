#pragma once
#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <algorithm>
#include "../lib/pole.h"
#include "../lib/player.h"
#include "../DataBase/MySQL.h"
#include "PowerMapper.h"
#include "request.h"


class Game {
public:

    Game(Lobby *lobby_) : lobby(lobby_) {}

    void StartGame();

    void CallPechkin(int playerId, const Message<GameMsgTypes> &msg);

    void LobbyShortCoordinates(int &line, int &cell) {
        lobby->incoming().wait();
        auto msg = lobby->incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::GameShortCoordinates) {
            msg >> line >> cell;
        } else {
            std::cout << "Error in short Coordinates!\n";
            return;
        }
    }

    void SetLeader(std::vector<Player *> &players, int playerside, int choice, Pole &pole, PowerMapper &mapper);

    void SendBreeds(Pole &pole, int numOfPlayers);

    void FrontRequest(Position *friendPosition);

    void MiddleRequest(Position *friendPosition);

    void BackRequest(Position *friendPosition);

    bool WaitForMessage();

    void ReceiveTarget(int &line, int &cell);

    void ReleasePower(int type, Position *position, int side);

    void SendBeforeHeroStats();

    void SendReject();

    vector<Breed> GetBreeds();

    bool FieldIsChanged(vector<Breed> before, vector<Breed> after);

private:
    int currentSide;
    MySQL sql;
    Deck allCards;
    int winner = -1;
    PowerMapper mapper;
    Pole pole;
    Lobby *lobby;
};

#endif