#include "Unit.h"
#include <iostream>
#include "configurations.cpp"
using std::cout;
using std::endl;
using std::to_string;

Unit::Unit(RenderWindow &_window) : DrawableBox(_window) 
{ 
}

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

void Unit::draw()
{
    window.draw(rect);
}

int Unit::getId()
{
    return Id;
}

void Unit::setFont(const string &fontPath)
{
    font.loadFromFile(fontPath);
    textName.setFont(font);
    textHP.setFont(font);
    textAttack.setFont(font);
    textAttack.setCharacterSize(20);
    textName.setCharacterSize(40);
    textName.setPosition(Vector2f(500, 400));
    textHP.setCharacterSize(20);
}

void Unit::setTextName(const string &str)
{
    textName.setColor(Color::Magenta);
    textName.setString(str);
}

void Unit::setTextHP(const int HP)
{
    health = HP;
    textHP.setColor(Color(255, 30, 90));
    string str = "HP: " + to_string(HP);
    textHP.setString(str);
}

void Unit::setTextAttack(const int &attack)
{
    this->attack = attack;
    textAttack.setColor(Color::Black);
    string str = "Attack: " + to_string(attack);
    textAttack.setString(str);
}

Unit::~Unit() {}