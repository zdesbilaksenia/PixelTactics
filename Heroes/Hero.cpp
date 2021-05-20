#include "Hero.h"

Hero::Hero(Card &card) : isAlive(true), isLeader(false), isDamaged(false), canBeAttackedFromDistance(true),
                         canBeAttackedClosely(true), canUseLeaderPower(false) {
    name = card.name;
    HP = card.HP;
    curHP = HP;
    strength = card.strength;
    curStrength = strength;
    ID = card.ID;
}

Hero::Hero(Hero &hero) {
    name = hero.name;
    HP = hero.HP;
    curHP = HP;
    strength = hero.strength;
    curStrength = hero.curStrength;
    ID = hero.ID;
    isDamaged = hero.isDamaged;
    isAlive = hero.isAlive;
    isLeader = hero.isLeader;
    canBeAttackedFromDistance = hero.canBeAttackedFromDistance;
    canBeAttackedClosely = hero.canBeAttackedClosely;
    canUseLeaderPower = hero.canUseLeaderPower;
}

void Hero::MakeLeader() {
    isLeader = true;
    if (!isDamaged) {
        HP += 10;
        curHP = HP;
    }
    canUseLeaderPower = true;
}

void Hero::ReduceHealth(int damage) {
    isDamaged = true;
    curHP -= damage;
    if (curHP <= 0)
        Die();
}

void Hero::Attack(Hero &hero, int damage) {
    if (hero.isAlive)
        hero.ReduceHealth(damage);
}

int Hero::GetID() {
    return ID;
}

bool Hero::IsDead() {
    return !isAlive;
}

void Hero::Die() {
    isAlive = false;
    canBeAttackedFromDistance = false;
    curHP = 0;
    isDamaged = true;
}

int Hero::GetHealth() {
    return HP;
}

int Hero::GetCurHealth() {
    return curHP;
}

void Hero::SetCurHealth(int health) {
    curHP = health;
    if (curHP == 0)
        Die();
    else
        isAlive = true;
}

int Hero::GetStrength() {
    return strength;
}

void Hero::SetCurStrength(int power) {
    curStrength = power;
}

int Hero::GetCurStrength() {
    return curStrength;
}

string Hero::GetName() {
    return name;
}

bool Hero::IsLeader() {
    return isLeader;
}

bool Hero::IsDamaged() {
    return isDamaged;
}

void Hero::ForbidDistantAttack() {
    if (isAlive)
        canBeAttackedFromDistance = false;
}

void Hero::ForbidCloseAttack() {
    if (isAlive)
        canBeAttackedClosely = false;
}

bool Hero::CanBeAttackedClosely() {
    return canBeAttackedClosely;
}

bool Hero::CanBeAttackedDistantly() {
    return canBeAttackedFromDistance;
}

bool Hero::CanUseLeaderPower() {
    return canUseLeaderPower;
}

bool Hero::ForbidLeaderPower() {
    canUseLeaderPower = false;
}