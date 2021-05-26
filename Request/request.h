#pragma once

#ifndef REQUEST_H
#define REQUEST_H

#include "pole.h"
#include "vector"
#include "Lobby.h"

using std::vector;

struct Breed {
    int HP;
    int Strength;
    int ID;
};

void SendBreeds(Pole &pole, int numOfPlayers);

std::vector<Breed> ReturnRequest(Pole &pole_);

vector<bool> AttackRequest(Pole &pole_, int side);

int AttackWasDone(int xAttack, int yAttack, int xAttacked, int yAttacked, int friendlyside, Pole &pole_);

void UseMagic(int xAttack, int yAttack, int friendlyside, Pole &pole_, int phase);

void FrontRequest(Position *friendPosition, Pole &pole, int friendlyside);

void MiddleRequest(Position *friendPosition, Pole &pole, int friendlyside);

void BackRequest(Position *friendPosition, Pole &pole, int friendlyside);

void Interception(Position *position, Pole &pole_);

void HPFromDead(Position *position, Pole &pole_);

void PlusPower(Position *friendPosition, Pole &pole, int friendlyside);

void InterceptionPlusPower(Position *friendPosition, Pole &pole);

void AttackDamageToHP(Position *friendPosition, Pole &pole, int friendlyside);

void TransferDamage(Position *friendPosition, Pole &pole, int friendlyside);

void Resurrect(Position *friendPosition, Pole &pole, int friendlyside);

void Castling(Position *friendPosition, Pole &pole, int friendlyside);

void PlusStrMinusHP(Position *friendPosition, Pole &pole);

void Resurrect(Position *friendPosition, Pole &pole, int friendlyside);

void LeaderMakeDistantAttack(Position *position, Pole &pole_, int friendlyside);

void PlusPowerInCloseAttack(Position *position, Pole &pole_, int friendlyside);

void ResAndGetDamage(Position *position, Pole &pole_, int friendlyside);

void KillWthDamage(Position *position, Pole &pole_, int friendlyside);

void HitAvangard(Position *position, Pole &pole_, int friendlyside);

void CastlingWithDead(Position *position, Pole &pole_, int friendlyside);

void PlusPowerByDead(Position *position, Pole &pole_, int friendlyside);

void DamageTwoRows(Position *position, Pole &pole_, int friendlyside);

void FrontAndBackGetDamage(Position *position, Pole &pole_, int friendlyside);

void MakeDistantAttack(Position *position, Pole &pole_, int friendlyside);

vector<bool> SendAllDeadFriends(Position *position, Pole &pole_, int friendlySide);

vector<bool> SendAllAliveEnemies(Position *position, Pole &pole_, int friendlyside);

vector<bool>  SendAllAliveFriends(Position *position, Pole &pole_, int friendlyside);

void DoubleStrengthAgainstLeader(Position *position, Pole &pole_, int friendlyside);

void HitFrontRowHeroes(Position *position, Pole &pole_, int friendlyside);

#endif

