#ifndef PIXELTACTICS_CARD_H
#define PIXELTACTICS_CARD_H

#include <string>

using std::string;

typedef struct Card {

    int ID;
    string name;
    int HP;
    int strength;
    string frontLinePower;
    string middleLinePower;
    string backLinePower;
    string leaderPower;

    Card() {};
} Card;

#endif //PIXELTACTICS_CARD_H
