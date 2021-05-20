#include "Unit.h"

Unit::Unit(RenderWindow &_window) : DrawableBox(_window) {}

void Unit::setCharacter() {}


void Unit::setHealth(int _health) 
{
    this->health = _health;
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