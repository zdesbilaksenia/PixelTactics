#include "Powers.h"

void PlusPower(vector<Hero *> &heroes) {
    if (heroes.size() == 2) {
        if (!heroes[1]->IsDead()) {
            heroes[1]->SetCurStrength(heroes[1]->GetCurStrength() + 5);
        }
    }
}

void Resurrect(vector<Hero *> &heroes) {
    if (heroes.size() == 2) {
        if (heroes[1]->IsDead()) {
            heroes[1]->SetCurHealth(heroes[1]->GetHealth());
        }
    }
}

void Castling(vector<Hero *> &heroes) {
    if (heroes.size() == 3) {
        if (!heroes[1]->IsDead() && !heroes[2]->IsDead()) {
            Hero temp = *heroes[1];
            *heroes[1] = *heroes[2];
            *heroes[2] = temp;
        }
    }
}

void HitFrontRowHeroes(vector<Hero *> &heroes) {
    if (heroes.size() == 4 &&
        for_each(heroes.begin() + 1, heroes.end(), [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        for_each(heroes.begin() + 1, heroes.end(), [](Hero *hero) {
            hero->ReduceHealth(3);
        });
    }
}

void TransferDamage(vector<Hero *> &heroes) {
    if (heroes.size() == 3 &&
        for_each(heroes.begin() + 1, heroes.end(),
                 [](Hero *hero) -> bool { return !hero->IsDead() && !hero->IsLeader(); })) {
        if (heroes[2]->IsDamaged()) {
            heroes[2]->SetCurHealth(heroes[2]->GetCurHealth() + 2);
            heroes[1]->ReduceHealth(2);
        }
    }
}

void AttackDamageToHP(vector<Hero *> &heroes) {
    if (heroes.size() == 2) {
        heroes[0]->Attack(*heroes[1], heroes[0]->GetStrength());
        heroes[0]->SetCurHealth(heroes[0]->GetCurHealth() + heroes[0]->GetStrength());
    }
}

void CopyPower(vector<Hero *> &heroes) {
    if (heroes.size() > 2) {
        if (!heroes[0]->IsDead() && !heroes[1]->IsDead()) {
            heroes.erase(heroes.begin());
            heroes[1]->middleLinePower(heroes);
        }
    }
}

void Interception(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && !heroes[1]->IsDead()) {
        heroes[1]->ForbidDistantAttack();
    }
}

void DeadCanAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 3) {
        if (heroes[1]->IsDead())
            heroes[1]->Attack(*heroes[2], 3);
    }
}

void PlusPowerInCloseAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && !heroes[0]->IsDead()) {
        heroes[0]->Attack(*heroes[1], heroes[0]->GetStrength() + 2);
    }
}

void PlusPowerByDead(vector<Hero *> &heroes) {
    if (for_each(heroes.begin() + 1, heroes.end(),
                 [](Hero *hero) -> bool { return hero->IsDead(); })) {
        if (!heroes[0]->IsDead())
            heroes[0]->SetCurHealth(heroes.size() - 1 + heroes[0]->GetCurHealth());
    }
}

void PlusPowerForDeath(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && !heroes[1]->IsDead()) {
        heroes[0]->SetCurStrength(heroes[0]->GetCurStrength() + 4);
        heroes[1]->Die();
    }
}

void CastlingWithDead(vector<Hero *> &heroes) {
    if (heroes.size() == 3) {
        if (!heroes[1]->IsDead() && heroes[2]->IsDead()) {
            Hero temp = *heroes[1];
            *heroes[1] = *heroes[2];
            *heroes[2] = temp;
        }
    }
}

void ResurrectAndGetDamage(vector<Hero *> &heroes) {
    if (heroes.size() == 2) {
        if (heroes[1]->IsDead()) {
            heroes[1]->SetCurHealth(heroes[1]->GetHealth());
            heroes[0]->ReduceHealth(heroes[1]->GetHealth());
        }
    }
}

void HitAvangard(vector<Hero *> &heroes) {
    if (heroes.size() <= 4) {
        for_each(heroes.begin() + 1, heroes.end(),
                 [](Hero *hero) { hero->ReduceHealth(4); });
    }
}

void MakeAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 3 && for_each(heroes.begin(), heroes.end(),
                                       [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        heroes[0]->Attack(*heroes[2], heroes[1]->GetStrength());
    }
}

void KillWithDamage(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && for_each(heroes.begin(), heroes.end(),
                                       [](Hero *hero) -> bool { return !hero->IsDead() && !hero->IsLeader(); })) {
        heroes[0]->ReduceHealth(3);
        heroes[1]->Die();
    }
}

void DoubleStrengthAgainstLeader(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && for_each(heroes.begin(), heroes.end(),
                                       [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        if (heroes[1]->IsLeader())
            heroes[0]->Attack(*heroes[1], heroes[0]->GetCurStrength() * 2);
    }
}

void TransferDamageFromLeader(vector<Hero *> &heroes) {
    if (heroes.size() == 3 && heroes[1]->IsLeader()) {
        heroes[1]->SetCurHealth(heroes[1]->GetCurHealth() + 5);
        heroes[2]->ReduceHealth(5);
    }
}

void DieAndAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 3 && !heroes[1]->IsDead() && heroes[2]->CanBeAttackedDistantly()) {
        heroes[1]->Attack(*heroes[2], heroes[1]->GetCurStrength());
        heroes[1]->Die();
    }
}

void DamageTwoColsInBothSquad(vector<Hero *> &heroes) {
    if (heroes.size() <= 7 && for_each(heroes.begin(), heroes.end(),
                                       [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        for_each(heroes.begin() + 1, heroes.end(),
                 [](Hero *hero) { hero->ReduceHealth(1); });
    }
}


void DamageTwoRowsInBothSquad(vector<Hero *> &heroes) {
    if (heroes.size() <= 7 && for_each(heroes.begin(), heroes.end(),
                                       [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        for_each(heroes.begin() + 1, heroes.end(),
                 [](Hero *hero) { hero->ReduceHealth(1); });
    }
}

void TransferDamageWithoutUsingAction(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && !heroes[0]->IsDead()) {
        if (heroes[1]->GetHealth() - heroes[1]->GetCurHealth() > 0) {
            heroes[0]->ReduceHealth(heroes[1]->GetHealth() - heroes[1]->GetCurHealth());
            heroes[1]->SetCurHealth(heroes[1]->GetHealth());
        }
    }
}

void InterceptionAndPlusPower(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && !heroes[0]->IsDead() && !heroes[1]->IsDead()) {
        heroes[0]->SetCurStrength(heroes[0]->GetCurStrength() + 2);
        heroes[1]->ForbidDistantAttack();
    }
}

void MakeDistantAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 2 && heroes[1]->CanBeAttackedDistantly()) {
        heroes[0]->Attack(*heroes[1], heroes[0]->GetCurStrength());
    }
}

void FrontAndBackAttackDistantly(vector<Hero *> &heroes) {
    if (heroes.size() == 5 && heroes[3]->CanBeAttackedDistantly() && heroes[4]->CanBeAttackedDistantly()) {
        heroes[1]->Attack(*heroes[3], heroes[1]->GetCurStrength());
        heroes[2]->Attack(*heroes[4], heroes[2]->GetCurStrength());
    }
}

void PowerFromCurDamage(vector<Hero *> &heroes) {
    if (heroes.size() == 1 && heroes[0]->IsDamaged() && heroes[0]->GetHealth() > heroes[0]->GetCurHealth()) {
        heroes[0]->SetCurStrength(heroes[0]->GetHealth() - heroes[0]->GetCurHealth());
    }
}

void AnotherHeroMakeDistantAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 3 && heroes[2]->CanBeAttackedDistantly() && !heroes[1]->IsDead()) {
        heroes[1]->Attack(*heroes[2], heroes[1]->GetCurStrength());
    }
}

void PlusStrengthMinusHP(vector<Hero *> &heroes) {
    if (heroes.size() == 1 && !heroes[0]->IsDead()) {
        heroes[0]->ReduceHealth(1);
        heroes[0]->SetCurStrength(heroes[0]->GetCurStrength() + 1);
    }
}

void LeaderMakeDistantAttack(vector<Hero *> &heroes) {
    if (heroes.size() == 3 && !heroes[0]->IsDead() && heroes[1]->IsLeader() && heroes[2]->CanBeAttackedDistantly()) {
        heroes[1]->Attack(*heroes[2], heroes[1]->GetCurStrength());
    }
}

// Leader

void ForbidDistantAttackForThreeHeroes(vector<Hero *> &heroes) {
    if (heroes.size() == 4 && heroes[0]->IsLeader() && heroes[0]->CanUseLeaderPower() &&
        for_each(heroes.begin(), heroes.end(),
                 [](Hero *hero) -> bool { return !hero->IsDead(); })) {
        for_each(heroes.begin(), heroes.end(),
                 [](Hero *hero) -> bool { hero->ForbidDistantAttack(); });
    }
}

Powers::Powers() {
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Рыцарь", &Interception));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Ведьма", &DeadCanAttack));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Алхимик", &PlusPower));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Берсерк", &InterceptionAndPlusPower));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Боец", &AttackDamageToHP));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Вампир", &TransferDamage));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Дубликатор", &CopyPower));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Жрица", &Resurrect));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Маг-дракон", &Castling));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Зверолов", &PlusStrengthMinusHP));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Оракул", &Interception));
    frontLinePowerMap.insert(pair<string, FrontLinePower>("Паладин", &LeaderMakeDistantAttack));

    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Рыцарь", &PlusPowerInCloseAttack));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Ведьма", &ResurrectAndGetDamage));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Алхимик", &KillWithDamage));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Берсерк", &MakeAttack));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Боец", &HitAvangard));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Вампир", &CastlingWithDead));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Дубликатор", &CopyPower));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Жрица", &PlusPowerByDead));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Маг-дракон", &DamageTwoRowsInBothSquad));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Зверолов", &InterceptionAndPlusPower));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Оракул", &FrontAndBackAttackDistantly));
    middleLinePowerMap.insert(pair<string, MiddleLinePower>("Паладин", &MakeDistantAttack));

    backLinePowerMap.insert(pair<string, BackLinePower>("Рыцарь", &DoubleStrengthAgainstLeader));
    backLinePowerMap.insert(pair<string, BackLinePower>("Ведьма", &DieAndAttack));
    backLinePowerMap.insert(pair<string, BackLinePower>("Алхимик", &PowerFromCurDamage));
    backLinePowerMap.insert(pair<string, BackLinePower>("Берсерк", &PlusPowerForDeath));
    backLinePowerMap.insert(pair<string, BackLinePower>("Боец", &DamageTwoColsInBothSquad));
    backLinePowerMap.insert(pair<string, BackLinePower>("Вампир", &TransferDamageFromLeader));
    backLinePowerMap.insert(pair<string, BackLinePower>("Дубликатор", &CopyPower));
    backLinePowerMap.insert(pair<string, BackLinePower>("Жрица", &TransferDamageWithoutUsingAction));
    backLinePowerMap.insert(pair<string, BackLinePower>("Маг-дракон", &HitFrontRowHeroes));
    backLinePowerMap.insert(pair<string, BackLinePower>("Зверолов", &AnotherHeroMakeDistantAttack));
    backLinePowerMap.insert(pair<string, BackLinePower>("Оракул", &MakeDistantAttack));
    backLinePowerMap.insert(pair<string, BackLinePower>("Паладин", &PlusPower));
}