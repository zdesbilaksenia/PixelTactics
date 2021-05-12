#pragma once

#include <string>
using std::string;

#include <SFML/Graphics.hpp>
using namespace sf;

//Потом, наверное, магию всякую добавлять
//Сделать сеттеры и геттеры
class Character
{
public:

    Character();

    string name;
    Texture* texture;
    int health;
    int attack;
};