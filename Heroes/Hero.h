#ifndef PIXELTACTICS_HERO_H
#define PIXELTACTICS_HERO_H

#include "Card.h"

class Hero {

private:
    int HP;
    string name;
    int strength;
    bool isAlive;
    bool isLeader;

    void (*frontLinePower)();

    void (*middleLinePower)();

    void (*backLinePower)();

    void (*leaderPower)();

public:
    Hero(Card card) {};

    void UseFrontLinePower() {};

    void UseMiddleLinePower() {};

    void UseBackLinePower() {};

    virtual void UseLeaderPower() {};

    void Attack(Hero hero) {};

    bool CanAttack() {};

    int GetHealth() {};

    void ReduceHealth() {};

    bool IsDead() {};

    void Die() {};

    string GetName() {};

    string GetStrength() {};

};

#endif //PIXELTACTICS_HERO_H
