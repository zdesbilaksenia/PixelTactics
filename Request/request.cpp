#include "request.h"
#include "pole.h"

void Interception(Position *position, Pole &pole_, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *push_pos;
    push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() + 1, position->GetSide());
    if (!(push_pos->isEmpty()) && !(push_pos->GetHero().IsDead()) && (push_pos->GetHero().CanBeAttackedDistantly())) {
        heroes.push_back(&(push_pos->GetHero()));
        if (heroes.size() > 1) {
            position->GetHero().frontLinePower(heroes);
            done = true;
        }
    }
}

void HPFromDead(Position *position, Pole &pole_, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && !(pole_.GetVector()[i]->isEmpty())) {
            heroes.push_back(&(position->GetHero()));
            break;
        }
    }
    if (heroes.size() > 1) {
        position->GetHero().frontLinePower(heroes);
        done = true;
    }
}

void InterceptionPlusPower(Position *position, Pole &pole_, int type, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *push_pos;
    push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() + 1, position->GetSide());
    if (!(push_pos->isEmpty()) && !(push_pos->GetHero().IsDead()) && (push_pos->GetHero().CanBeAttackedDistantly())) {
        heroes.push_back(&(push_pos->GetHero()));
        if (type == 0) {
            if (heroes.size() > 1) {
                position->GetHero().frontLinePower(heroes);
                done = true;
            }
        } else if (type == 1) {
            if (heroes.size() > 1) {
                position->GetHero().middleLinePower(heroes);
                done = true;
            }
        }
    }
}

void PlusStrMinusHP(Position *position, Pole &pole_) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    position->GetHero().frontLinePower(heroes);
}

void HitAvangard(Position *position, Pole &pole_, int friendlyside, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < 3; i++) {
        Position *chosen_pos = pole_.GetPosition(i, 0, friendlyside ? 0 : 1);
        heroes.push_back(&(chosen_pos->GetHero()));
    }
    if (heroes.size() > 1) {
        position->GetHero().middleLinePower(heroes);
        done = true;
    }
}

void PlusPowerByDead(Position *position, Pole &pole_, int friendlyside, bool &done) {
    std::vector<Hero *> heroes;
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetSide() == friendlyside) &&
            pole_.GetVector()[i]->GetHero().GetID() != -1) {
            Position *chosen_pos = pole_.GetPosition(pole_.GetVector()[i]->GetCell(), pole_.GetVector()[i]->GetLine(),
                                                     friendlyside);
            heroes.push_back(&(chosen_pos->GetHero()));
        }
    }
    if (heroes.size() > 1) {
        position->GetHero().middleLinePower(heroes);
        done = true;
    }
}

void FrontAndBackGetDamage(Position *position, Pole &pole_, int friendlyside, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *push_pos;

    if (position->GetLine() != 0) {
        push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() - 1, position->GetSide());
        if ((!push_pos->isEmpty()) && !(push_pos->GetHero().IsDead())) {
            heroes.push_back(&(push_pos->GetHero()));
        }
    }

    if (position->GetLine() != 2) {
        push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() + 1, position->GetSide());
        if ((!push_pos->isEmpty()) && !(push_pos->GetHero().IsDead())) {
            heroes.push_back(&(push_pos->GetHero()));
        }
    }

    if (heroes.size() > 1) {
        position->GetHero().middleLinePower(heroes);
        done = true;
    }
}

vector<bool> MakeDistantAttack(Pole &pole_, int friendlyside) {
    std::vector<bool> HeroesCanBeAttackedDistantly;
    HeroesCanBeAttackedDistantly.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            pole_.GetVector()[i]->GetHero().CanBeAttackedDistantly() &&
            pole_.GetVector()[i]->GetSide() != friendlyside) {
            HeroesCanBeAttackedDistantly.push_back(true);
        } else {
            HeroesCanBeAttackedDistantly.push_back(false);
        }
    }
    return HeroesCanBeAttackedDistantly;
}

void DamageTwoRows(Position *position, Pole &pole_, Position *target, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetLine() == target->GetLine()))
            heroes.push_back(&pole_.GetVector()[i]->GetHero());
    }
    if (heroes.size() > 1) {
        position->GetHero().middleLinePower(heroes);
        done = true;
    }
}

void DamageTwoCols(Position *position, Pole &pole_, Position *target, bool &done) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetCell() == target->GetCell())) {
            heroes.push_back(&pole_.GetVector()[i]->GetHero());
        }
    }
    if (heroes.size() > 1) {
        position->GetHero().backLinePower(heroes);
        done = true;
    }
}

vector<bool> SendAllAliveEnemies(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) &&
            (!pole_.GetVector()[i]->isEmpty() && (pole_.GetVector()[i]->GetSide() != friendlyside)) &&
            !(pole_.GetVector()[i]->GetHero().IsLeader())) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    return HeroesAlive;
}

vector<bool> SendAllAliveFriends(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && !(pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() == friendlyside) && !(pole_.GetVector()[i]->GetHero().IsLeader()) &&
            pole_.GetVector()[i]->GetHero().GetID() != heroes[0]->GetID()) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    return HeroesAlive;
}

vector<bool> SendAllDeadFriends(Position *position, Pole &pole_, int friendlyside) {
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() == friendlyside)) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    return HeroesAlive;
}

void DoubleStrengthAgainstLeader(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *chosen_pos = pole_.GetPosition(1, 1, friendlyside ? 0 : 1);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().backLinePower(heroes);
}