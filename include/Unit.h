#pragma once

#include "DrawableBox.h"
#include <string>
#include <boost/log/trivial.hpp>
using namespace boost::log;
using std::string;

#include <iostream>
using namespace std;

class Unit : public DrawableBox
{
public:
    Unit(RenderWindow &_window);
    //===========================================================================================================
    //  Наверное, персонажей тоже нужно будет хранить в отдельном классе.
    //
    //      В этом классе будут храниться:
    //          Имя персонажа
    //          Изображение для текстуры
    //          Описание (текстовое)
    //          Характеристики (сила, здоровье)
    //          Магия и т.п.
    //
    //  А в самом юните будет уже "динамическая" работа с персонажем - будут отниматься ХП и что-нибудь ещё.
    //===========================================================================================================
    void setCharacter();

    void setData(const int &Id, const string &_name, const string _description[3], const int &_health, const int &_attack);

    int getHealth();
    int getAttack();

    void draw() override;
    int getId()
    {
        return Id;
    }

    void setFont(const string &fontPath) override
    {
        font.loadFromFile(fontPath);
        textName.setFont(font);
        textHP.setFont(font);
        textAttack.setFont(font);
        textAttack.setCharacterSize(40);
        textName.setCharacterSize(40);
        textHP.setCharacterSize(40);
    }
    //==================================
    void setTextName(const string &str)
    {
        textName.setColor(Color::Magenta);
        textName.setString(str);
    }
    //=================================
    void setTextHP(const int HP)
    {
        health = HP;
        textHP.setColor(Color::Red);
        string str = "HP: " + to_string(HP);
        textHP.setString(str);
    }
    //=================================
    void setTextAttack(const int &attack)
    {
        this->attack = attack;
        textAttack.setColor(Color::Magenta);
        string str = "Attack: " + to_string(attack);
        textAttack.setString(str);
    }
    //=================================

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
};