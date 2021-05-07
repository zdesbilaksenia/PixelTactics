#include "Powers.h"

void PlusPower(string &str) {

    str = "plus power";
}

void Resurrect(string &str) {

    str = "resurrect";
}

void AttackerGetsDamage(string &str) {

    str = "attacker gets damage";
}

void NoDamageFromDistantAttack(string &str) {

    str = "no damage from distant attack";
}

void HitFrontRowHeroes(string &str) {

    str = "hit front row hero";
}

void TransferDamage(string &str) {

    str = "transfer damage";
}

Powers::Powers() {

    frontLinePowerVector.push_back(&PlusPower);
    middleLinePowerVector.push_back(&Resurrect);
    backLinePowerVector.push_back(&HitFrontRowHeroes);
    leaderPowerVector.push_back(&TransferDamage);

    frontLinePowerVector.push_back(&PlusPower);
    middleLinePowerVector.push_back(&Resurrect);
    backLinePowerVector.push_back(&HitFrontRowHeroes);
    leaderPowerVector.push_back(&TransferDamage);

}