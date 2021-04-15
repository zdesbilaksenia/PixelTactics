#ifndef PIXELTACTICS_CARD_H
#define PIXELTACTICS_CARD_H

using std::string;

class Card {

private:
    int ID;
    string name;
    int HP;
    int strength;

    void (*frontLinePower)();

    void (*middleLinePower)();

    void (*backLinePower)();

    void (*leaderPower)();

public:
    Card() {};

    void MapPowers(void (*_frontLinePower)(), void (*_middleLinePower)(), void (*_backLinePower)(), void (*_leaderPower)()) {};

    void SetHP(int _HP) {};

    void SetName(string _name) {};

    void SetStrength(int _strength) {};

    int GetHP() {};

    string GetName() {};

    int GetStrength() {};

};

#endif //PIXELTACTICS_CARD_H
