#include "Unit.h"

Unit::Unit(RenderWindow &_window) : DrawableBox(_window) {}

void Unit::setCharacter() {}

void Unit::setData(const int& _Id, const string& _name, const string _description[3], const int& _health, const int& _attack)
{
    this->Id = _Id;
    this->name = _name;
    this->description[0] = _description[0];
    this->description[1] = _description[1];
    this->description[2] = _description[2];
    this->health = _health;
    this->attack = _attack;
}

int Unit::getHealth()
{
    return this->health;
}

int Unit::getAttack()
{
    return this->attack;
}

void Unit::draw()
{
    window.draw(rect);
}

Unit::~Unit() {}