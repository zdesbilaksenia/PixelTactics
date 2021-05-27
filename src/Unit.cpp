#include "Unit.h"
#include <iostream>
#include "configurations.cpp"
using namespace std;

Unit::Unit(RenderWindow &_window) : DrawableBox(_window) {}

void Unit::setCharacter() {}

void Unit::setData(const int &_Id, const string &_name, const string _description[3], const int &_health, const int &_attack)
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

void Unit::draw(const Mouse &mouse)
{
    textHP.setPosition(Vector2f(this->getPosX() - 20, this->getPosY() + tileHeight + 10));
    textAttack.setPosition(Vector2f(this->getPosX() + 40, this->getPosY() + tileHeight + 10));
    window.draw(rect);
    if (rect.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
    {
        window.draw(textName);
    }

    window.draw(textHP);
    window.draw(textAttack);
}

Unit::~Unit() {}