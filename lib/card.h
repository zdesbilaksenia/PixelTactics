#pragma once
#ifndef CARD_H
#define CARD_H

#include <iostream>

class Card;

class Breed{
public:
    Card* NewCard();
    Breed(Breed* parent, int health, int strength);

    int GetHealth();
    int GetStrength();

private:
    Breed* parent_;
    int health_; 
    int strength_;
};

class Card{

friend class Breed;
public:
    Breed& GetBreed();
    
private:
    Card(Breed& breed);
    int health;
    int strength;
    Breed& breed;
};
#endif