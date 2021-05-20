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

void PlusPower(vector<Hero *> &heroes);

void Resurrect(vector<Hero *> &heroes);

void Castling(vector<Hero *> &heroes);

void HitFrontRowHeroes(vector<Hero *> &heroes);

void TransferDamage(vector<Hero *> &heroes);

void AttackDamageToHP(vector<Hero *> &heroes);

void CopyPower(vector<Hero *> &heroes);

void Interception(vector<Hero *> &heroes);

void DeadCanAttack(vector<Hero *> &heroes);

void ProtectLeader(vector<Hero *> &heroes);

void PlusPowerInCloseAttack(vector<Hero *> &heroes);

void TransferDamageFromLeader(vector<Hero *> &heroes);

void PlusPowerByDead(vector<Hero *> &heroes);

void PlusPowerForDeath(vector<Hero *> &heroes);

void CastlingWithDead(vector<Hero *> &heroes);

void ResurrectAndGetDamage(vector<Hero *> &heroes);

void ReplaceHeroByDuplicator(vector<Hero *> &heroes);

void HitAvangard(vector<Hero *> &heroes);

void MakeAttack(vector<Hero *> &heroes);

void KillWithDamage(vector<Hero *> &heroes);

void DoubleStrengthAgainstLeader(vector<Hero *> &heroes);


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
