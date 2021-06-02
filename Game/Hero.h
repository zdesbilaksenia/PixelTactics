#pragma once
#ifndef HERO_H
#define HERO_H

#include "Card.h"
#include <vector>
#include <string>

class Hero{
private:
    int ID;
    int HP;
    int texture;
    int curHP;
    std::string name;
    int strength;
    int curStrength;
    bool isAlive;
    bool isLeader;
    bool isDamaged;
    bool canBeAttackedFromDistance;
    bool canBeAttackedClosely;
    bool canUseLeaderPower;

public:
    Hero();

    explicit Hero(Card &card);

    Hero(Hero &hero);

    void MakeLeader();

    void (*frontLinePower)(std::vector<Hero *> &heroes);

    void (*middleLinePower)(std::vector<Hero *> &heroes);

    void (*backLinePower)(std::vector<Hero *> &heroes);

    void (*leaderPower)(std::vector<Hero *> &heroes);

    void Attack(Hero &hero, int damage);

    int GetHealth();

    int GetCurHealth();

    void SetCurHealth(int HP);

    int GetID();

    int GetTexture();

    void ReduceHealth(int damage);

    bool Empty();

    bool IsDead();

    bool IsLeader();

    void Die();

    std::string GetName();

    int GetStrength();

    void SetCurStrength(int power);

    int GetCurStrength();

    bool IsDamaged();

    void ForbidDistantAttack();

    void ForbidCloseAttack();

    bool CanBeAttackedClosely();

    bool CanBeAttackedDistantly();

    bool CanUseLeaderPower();

    bool ForbidLeaderPower();

};
#endif