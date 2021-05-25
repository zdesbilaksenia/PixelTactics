#pragma once
#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <string>

typedef struct Card {

    int ID;
    int texture;
    std::string name;
    int HP;
    int strength;
    std::string frontLinePower;
    std::string middleLinePower;
    std::string backLinePower;
} Card;

#endif