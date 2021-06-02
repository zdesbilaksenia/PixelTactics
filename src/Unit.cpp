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
    textName.setCharacterSize(20);
    textName.setPosition(Vector2f(textDescPosX + 20, infoRectPosY + 20));
    textName.setColor(Color(80, 102, 255));
    textAttack.setCharacterSize(20);
    textAttack.setColor(Color(95, 12, 172));
    textHP.setCharacterSize(20);
    textHP.setColor(Color(255, 30, 90));
    for (int i = 0; i < 3; ++i)
    {
        textDescriptions[i].setCharacterSize(15);
        textDescriptions[i].setColor(Color(95, 12, 172));
        textDescriptions[i].setPosition(Vector2f(textDescPosX, textDescPosY + 20 * i));
    }
}

void Unit::setCharacter() {}

void Unit::setData(const int &_Id, const string &strName, const string strDescs[3], const int &_health, const int &_attack)
{
    this->Id = _Id;
    textName.setString(String::fromUtf8(strName.begin(), strName.end()));
    for (int i = 0; i < 3; ++i)
    {
        textDescriptions[i].setString(String::fromUtf8(strDescs[i].begin(), strDescs[i].end()));
    }
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
    if (rect.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y))
    {
        window.draw(textName);
        for (int i = 0; i < 3; ++i)
        {
            window.draw(textDescriptions[i]);
        }
    }
    window.draw(rect);
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
    for (int i = 0; i < 3; ++i)
    {
        textDescriptions[i].setFont(font);
    }
}

void Unit::setTextHP(const int HP)
{
    health = HP;
    string str = to_string(HP);
    textHP.setString(str);
}

void Unit::setTextAttack(const int &attack)
{
    this->attack = attack;
    string str = to_string(attack);
    textAttack.setString(str);
}

void Unit::setHeartAndSwordTx(Texture *heartTx, Texture *swordTx)
{
    heartRect.setTexture(heartTx);
    swordRect.setTexture(swordTx);
}

Unit::~Unit() {}