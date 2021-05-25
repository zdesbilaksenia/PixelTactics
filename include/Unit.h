#pragma once

#include "DrawableBox.h"
#include <string>
#include <boost/log/trivial.hpp>
using namespace boost::log;
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

    void setData(const int& Id, const string& _name, const string _description[3], const int& _health, const int& _attack);

    int getHealth();
    int getAttack();

    void draw() override;
    int getId()
    {
        return Id;
    }
    
    ~Unit();

private:

    int Id;
    string name;
    string description[3];
    int health;
    int attack;
};