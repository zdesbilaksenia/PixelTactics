#pragma once
#ifndef HERO_H
#define HERO_H

#include "card.h"


class Hero{
public:
    Hero();
    Hero(Card* card);
    //Hero& operator = (const Hero& hero);
    int GetHealth(){
        return health_;
    }
    int GetStrength(){
        return strength_;
    }
    void ReduceHealth(int damage){
        health_ -= damage;
    }
    /*void Attack(Position& Enemy){
        Enemy.GetHero().ReduceHealth(strength_);
        if(Enemy.GetHero().GetHealth() <=0){
            Enemy.RemoveHero();
        }
    }*/
private:
    int health_;
    int strength_;
};
#endif