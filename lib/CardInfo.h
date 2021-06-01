#pragma once

#include <cstring>
#include <iostream>

struct CardInfo {
    int ID;
    int textureId;
    std::string name;
    int HP;
    int strength;
    std::string frontLinePower;
    std::string middleLinePower;
    std::string backLinePower;
};

struct Breed
{
    int HP;
    int strength;
    int cardID;
    bool distant;
};