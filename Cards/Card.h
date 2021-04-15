#ifndef PIXELTACTICS_CARD_H
#define PIXELTACTICS_CARD_H

using std::string;

typedef void (*FrontLinePower)();

typedef void (*MiddleLinePower)();

typedef void (*BackLinePower)();

typedef void (*LeaderPower)();

class Card {

private:
    int ID;
    string name;
    int HP;
    int strength;

    FrontLinePower frontLinePower() {};

    MiddleLinePower middleLinePower() {};

    BackLinePower backLinePower() {};

    LeaderPower leaderPower() {};

public:
    Card() {};

    void SetHP(int _HP) {};

    void SetName(string _name) {};

    void SetStrength(int _strength) {};

    int GetHP() {};

    string GetName() {};

    int GetStrength() {};

    FrontLinePower GetFrontLinePower() {};

    FrontLinePower GetMiddleLinePower() {};

    FrontLinePower GetBackLinePower() {};

    FrontLinePower GetLeaderPower() {};
};

#endif //PIXELTACTICS_CARD_H
