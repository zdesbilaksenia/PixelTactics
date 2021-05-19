#pragma once
#ifndef POSITION_H
#define POSITION_H

#include "hero.h"
#include "card.h"


class Position{
public:
    Position();
    Position(int cell, int line, int side);

    //Пустая ли клетка?
    bool isEmpty();
    //Получить героя с клетки
    //Hero& GetHero();
    //Информация о позиции
    //Поставить героя
    void SetHero(Card* card);
    void RemoveHero();
    
    int GetCell();
    int GetLine();
    int GetSide();
    Hero& GetHero();

    void InfoPosition();

private:
    //Дальняя клетка - 1 Средняя - 2 Ближняя - 3
    unsigned int cell_;
    //Передняя линия - 1 Средняя -2 Задняя - 3
    unsigned int line_;
    //Сторона сражения
    unsigned int side_;
    //Состояние клетки
    bool empty_;
    //Герой на данной клетке
    Hero hero_;
};
#endif