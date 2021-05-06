#ifndef PIXELTACTICS_POWERS_H
#define PIXELTACTICS_POWERS_H

#include <vector>
#include <string>

using std::string;
using std::vector;

typedef void (*FrontLinePower)(string &);

typedef void (*MiddleLinePower)(string &);

typedef void (*BackLinePower)(string &);

typedef void (*LeaderPower)(string &);

void PlusPower(string &str);

void Resurrect(string &);

void HitFrontRowHeroes(string &str);

void TransferDamage(string &str);

class Powers {

private:
    vector<FrontLinePower> frontLinePowerVector;
    vector<MiddleLinePower> middleLinePowerVector;
    vector<BackLinePower> backLinePowerVector;
    vector<LeaderPower> leaderPowerVector;
public:
    Powers();

    FrontLinePower ReturnFrontLinePower(int ID) {
        return frontLinePowerVector[ID];
    }

    MiddleLinePower ReturnMiddleLinePower(int ID) {
        return middleLinePowerVector[ID];
    }

    BackLinePower ReturnBackLinePower(int ID) {
        return backLinePowerVector[ID];
    }

    LeaderPower ReturnLeaderPower(int ID) {
        return leaderPowerVector[ID];
    }
};

#endif //PIXELTACTICS_POWERS_H
