#ifndef PIXELTACTICS_CARD_H
#define PIXELTACTICS_CARD_H

#include <string>
#include "Powers.h"

using std::string;

typedef struct Card {

    int ID;
    string name;
    int HP;
    int strength;
    FrontLinePower frontLinePower;
    MiddleLinePower middleLinePower;
    BackLinePower backLinePower;
    LeaderPower leaderPower;

    Card() {};
} Card;

#endif //PIXELTACTICS_CARD_H
