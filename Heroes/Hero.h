#ifndef PIXELTACTICS_HERO_H
#define PIXELTACTICS_HERO_H

#include "Card.h"
#include <vector>

using std::vector;

class Hero {

private:
    int ID;
    int HP;
    int curHP;
    string name;
    int strength;
    int curStrength;
    bool isAlive;
    bool isLeader;
    bool isDamaged;
    bool canBeAttackedFromDistance;
    bool canBeAttackedClosely;
    bool canUseLeaderPower;

public:
    Hero() {};

    explicit Hero(Card &card);

    Hero(Hero &hero);

    void MakeLeader();

    void (*frontLinePower)(vector<Hero *> &heroes);

    void (*middleLinePower)(vector<Hero *> &heroes);

    void (*backLinePower)(vector<Hero *> &heroes);

    void (*leaderPower)(vector<Hero *> &heroes);

    void Attack(Hero &hero, int damage);

    int GetHealth();

    int GetCurHealth();

    void SetCurHealth(int HP);

    int GetID();

    void ReduceHealth(int damage);

    bool IsDead();

    bool IsLeader();

    void Die();

    string GetName();

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


#endif //PIXELTACTICS_HERO_H
