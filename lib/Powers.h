#ifndef PIXELTACTICS_POWERS_H
#define PIXELTACTICS_POWERS_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include "hero.h"

using std::string;
using std::vector;
using std::for_each;
using std::map;
using std::pair;

typedef void (*FrontLinePower)(vector<Hero *> &);

typedef void (*MiddleLinePower)(vector<Hero *> &);

typedef void (*BackLinePower)(vector<Hero *> &);

typedef void (*LeaderPower)(vector<Hero *> &);

class Powers {

private:
    map<string, FrontLinePower> frontLinePowerMap;
    map<string, MiddleLinePower> middleLinePowerMap;
    map<string, BackLinePower> backLinePowerMap;
    map<string, LeaderPower> leaderPowerMap;
public:
    Powers();

    FrontLinePower ReturnFrontLinePower(const string &name) {
        return frontLinePowerMap.find(name)->second;
    }

    MiddleLinePower ReturnMiddleLinePower(const string &name) {
        return middleLinePowerMap.find(name)->second;
    }

    BackLinePower ReturnBackLinePower(const string &name) {
        return backLinePowerMap.find(name)->second;
    }

    LeaderPower ReturnLeaderPower(const string &name) {
        return leaderPowerMap.find(name)->second;
    }
};

#endif //PIXELTACTICS_POWERS_H
