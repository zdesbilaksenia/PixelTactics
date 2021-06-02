#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#include "../Game/Pole.h"
#include "vector"
#include "Lobby.h"

using std::vector;

struct Breed {
    int HP;
    int Strength;
    int ID;
    bool distant;
};

void Interception(Position *position, Pole &pole_, bool &done);

void HPFromDead(Position *position, Pole &pole_, bool &done);

void InterceptionPlusPower(Position *position, Pole &pole_, int type, bool &done);

void Resurrect(Position *friendPosition, Pole &pole, int friendlyside);

void PlusStrMinusHP(Position *friendPosition, Pole &pole);

void Resurrect(Position *friendPosition, Pole &pole, int friendlyside);

void HitAvangard(Position *position, Pole &pole_, int friendlyside, bool &done);

void PlusPowerByDead(Position *position, Pole &pole_, int friendlyside, bool &done);

void DamageTwoRows(Position *position, Pole &pole_, Position *target, bool &done);

void DamageTwoCols(Position *position, Pole &pole_, Position *target, bool &done);

void FrontAndBackGetDamage(Position *position, Pole &pole_, int friendlyside, bool &done);

vector<bool> MakeDistantAttack(Pole &pole_, int friendlyside);

vector<bool> SendAllDeadFriends(Position *position, Pole &pole_, int friendlySide);

vector<bool> SendAllAliveEnemies(Position *position, Pole &pole_, int friendlyside);

vector<bool> SendAllAliveFriends(Position *position, Pole &pole_, int friendlyside);

void DoubleStrengthAgainstLeader(Position *position, Pole &pole_, int friendlyside);

#endif

