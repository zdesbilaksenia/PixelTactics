#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include "Deck.h"
#include "Pole.h"
#include "Hero.h"



class Player{
public:
    Player(Deck& deck,int side);
    //Player(const Player& other);
    //Player& operator = (const Player& other);
    //~Player();

    //Выбрать карту из руки 
    Card ChooseCard(int CardIndex);
    //Сыграть карту
    //Взять карту из колоды
    void DrawCard();
    //Position& ChooseCoordinates(int cell, int line, Pole pole);
    //void PlayCard(Pole pole);
    //Аттаковать своим героем
    //void Attack(Position friendlyHero);
    //Убрать тело с поля
    //void RemoveDead(Position Corpse);
    //Закончить ход
    //void EndTurn();
    //Узнать позицию лидера
    //Position GetLeaderPosition();
    //Узнать сторону игрока
    int GetSide();
    Deck& GetDeck();
    void StartingHand();
    Deck& GetHand();
    void Attack(Position& FriendlyHero, Position& EnemyHero);
    bool MeleeAttackCheck(Position* EnemyHero, Pole& pole);

private:
    //Рука Карт
    Deck hand_;
    //Макс. Размер руки
    //unsigned int maxHandSize;
    //Действия на ход (У каждого игрока по 2)
    //unsigned int movesAmount;
    //Колода игрока
    Deck deck_;
    //Позиция лидера
    //Position leader;
    //Первый игрок или второй
    int side_;
}; 
#endif