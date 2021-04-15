#include <iostream>
#include <string>
#include "Hero.h"

class Position{
public:
    Position();
    Position(int cell_, int line_, int side_, bool empty_ = true);
    Position(const Position& other);
    Position& operator = (const Position& other);
    ~Position();

    //Пустая ли клетка?
    bool isEmpty();
    //Получить героя с клетки
    Hero& GetHero();
    //Информация о позиции
    Position GetPosition();
    //Поставить героя
    Position SetHero();
    
    int GetCell(){return cell};
    int GetLine(){return line};
    int GetSide(){return side};

private:
    //Дальняя клетка - 1 Средняя - 2 Ближняя - 3
    unsigned int cell;
    //Передняя линия - 1 Средняя -2 Задняя - 3
    unsigned int line;
    //Сторона сражения
    unsigned int side;
    //Состояние клетки
    bool empty;
    //Герой на данной клетке
    Hero hero;
};

/*class Hero {
public:
    Hero();
    Hero(std::string name, unsigned int hp, unsigned int _strength, Position heroPosition, unsigned int side);
    Hero(const Hero& other);
    Hero& operator = (const Hero& other);
    ~Hero();

    void Attack(Hero Enemy);
    void UseAbility();

private:
    unsigned int _hp;
    std::string _name;
    unsigned int _strength;
    Position _heroPosition;
    unsigned int side;
};*/