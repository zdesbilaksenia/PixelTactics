#ifndef PIXELTACTICS_POWERS_H
#define PIXELTACTICS_POWERS_H

using std::vector;

#include "Card.h"

typedef void (*FrontLinePower)();

typedef void (*MiddleLinePower)();

typedef void (*BackLinePower)();

typedef void (*LeaderPower)();

void PlusPower();

void Resurrect();

void HitFrontRowHeroes();

void HitOneColumn();

void TransferDamage();

class Powers {

private:
    vector<FrontLinePower> frontLinePowerVector;
    vector<MiddleLinePower> middleLinePowerVector;
    vector<BackLinePower> backLinePowerVector;
    vector<LeaderPower> leaderPowerVector;
public:
    void FillVectors() {};

    void ReturnFrontPower(Card &card) {};
    void ReturnMiddlePower(Card &card) {};
    void ReturnBackPower(Card &card) {};
    void ReturnLeaderPower(Card &card) {};
};

/*void Powers::FillVectors() {

    frontLinePowerVector.push_back(PlusPower);
    frontLinePowerVector.push_back(Resurrect);

    middleLinePowerVector.push_back(HitFrontRowHeroes);

    backLinePowerVector.push_back(HitOneColumn);

    leaderPowerVector.push_back(TransferDamage);

}*/

#endif //PIXELTACTICS_POWERS_H
