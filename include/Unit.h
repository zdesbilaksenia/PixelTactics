#pragma once

#include "DrawableBox.h"
#include <string>
using std::string;

class Unit : public DrawableBox
{
public:

    Unit(RenderWindow& _window);
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

    void setData(string _name, string _description[3], int _health, int _attack);

    int getHealth();
    int getAttack();

    void draw() override;

    ~Unit();

private:

    string name;
    string description[3];
    int health;
    int attack;
    //Label healthLabel;
    //Label attackLabel;
};