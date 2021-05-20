#pragma once
#ifndef POSITION_H
#define POSITION_H

#include "Powers.h"


class Position{
public:
    Position():cell_(0),line_(0),side_(0),empty_(true){
        Hero temp;
        hero_ = temp;
    }
    Position(int cell, int line, int side):cell_(cell),line_(line),side_(side),empty_(true){
        Hero temp;
        hero_ = temp;
    }
    Position(const Position& position){
        cell_ = position.cell_;
        line_ = position.line_;
        side_ = position.side_;
        empty_ = position.empty_;
        hero_ = position.hero_;
    }

    //Пустая ли клетка?
    bool isEmpty();
    //Получить героя с клетки
    //Hero& GetHero();
    //Информация о позиции
    //Поставить героя
    void SetHero(Card card);
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