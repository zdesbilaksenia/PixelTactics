#include "Unit.h"
#include <iostream>
#include "configurations.cpp"
using std::cout;
using std::endl;
using std::to_string;

Unit::Unit(RenderWindow &_window) : DrawableBox(_window)
{
    heartRect.setSize(Vector2f(statPictureWidth, statPictureHeight));
    swordRect.setSize(Vector2f(statPictureWidth, statPictureHeight));
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
    heartRect.setPosition(Vector2f(this->getPosX() + 10, this->getPosY() + tileHeight + 10));
    textHP.setPosition(Vector2f(this->getPosX() + 40, this->getPosY() + tileHeight + 10));
    swordRect.setPosition(Vector2f(this->getPosX() + 60, this->getPosY() + tileHeight + 10));
    textAttack.setPosition(Vector2f(this->getPosX() + 90, this->getPosY() + tileHeight + 10));
    window.draw(rect);
    if (rect.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
    {
        window.draw(textName);
    }
    window.draw(heartRect);
    window.draw(textHP);
    window.draw(swordRect);
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
    textName.setCharacterSize(40);
    textAttack.setCharacterSize(30);
    textHP.setCharacterSize(30);
    textName.setPosition(Vector2f(500, 400));
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
    string str = to_string(HP);
    textHP.setString(str);
}

void Unit::setTextAttack(const int &attack)
{
    this->attack = attack;
    textAttack.setColor(Color::Black);
    string str = to_string(attack);
    textAttack.setString(str);
}

void Unit::setHeartAndSwordTx(Texture *heartTx, Texture *swordTx)
{
    heartRect.setTexture(heartTx);
    swordRect.setTexture(swordTx);
}

Unit::~Unit() {}