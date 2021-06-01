#pragma once

#include "DrawableBox.h"
#include <string>
#include <boost/log/trivial.hpp>
using namespace boost::log;
using std::string;

#include <iostream>
using std::cout;
using std::endl;

class Unit : public DrawableBox
{
public:
    Unit(RenderWindow &_window);
    void setCharacter();
    void setData(const int &Id, const string &_name, const string _description[3], const int &_health, const int &_attack);
    int getHealth();
    int getAttack();
    void draw(const Mouse &_mouse);
    void draw();
    int getId();
    void setFont(const string &fontPath) override;
    void setTextName(const string &str);
    void setTextHP(const int HP);
    void setTextAttack(const int &attack);
    void setHeartAndSwordTx(Texture* heartTx, Texture* swordTx);

    ~Unit();

private:
    int Id;
    string name;
    string description[3];
    int health;
    int attack;

    Text textName;
    Text textHP;
    Text textAttack;

    RectangleShape swordRect;
    RectangleShape heartRect;
};