#include "request.h"
#include "pole.h"

void Interception(Position *position, Pole &pole_) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *push_pos;
    push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() + 1, position->GetSide());
    if (!(push_pos->isEmpty()) && !(push_pos->GetHero().IsDead())) {
        heroes.push_back(&(push_pos->GetHero()));
        position->GetHero().frontLinePower(heroes);
    }
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, который стоит за ним
    //персонаж_чью_способность_используем->frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void HPFromDead(Position *position, Pole &pole_) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (pole_.GetVector()[i]->GetHero().IsDead()) {
            heroes.push_back(&(position->GetHero()));
            break;
        }
    }
    if (heroes.size() > 1) {
        position->GetHero().frontLinePower(heroes);
    }

//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем
    // найти, есть ли в отряде играющего мертвые, если есть: персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void InterceptionPlusPower(Position *position, Pole &pole_) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    Position *push_pos;
    push_pos = pole_.GetPosition(position->GetCell(), position->GetLine() + 1, position->GetSide());
    if (!(push_pos->isEmpty()) && !(push_pos->GetHero().IsDead())) {
        heroes.push_back(&(push_pos->GetHero()));
        position->GetHero().frontLinePower(heroes);
    }
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, который стоит за ним
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void PlusStrMinusHP(Position *position, Pole &pole_) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void PlusPower(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty())) {
            HeroesAlive.push_back(true);
            count++;
        } else {
            HeroesAlive.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);

    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 помечены живые персонажи своего отряда и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void AttackDamageToHP(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty())) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 помечены живые персонажи отряда противника и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void TransferDamage(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesDamaged;
    HeroesDamaged.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDamaged()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (!pole_.GetVector()[i]->GetHero().IsDead())) {
            HeroesDamaged.push_back(true);
            count++;
        } else {
            HeroesDamaged.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 помечены живые персонажи, у которых метод idDamaged() вернул true и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void Resurrect(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesDead;
    HeroesDead.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty())) {
            HeroesDead.push_back(true);
            count++;
        } else {
            HeroesDead.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 мертвые персонажи на поле и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void Castling(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAlive;
    HeroesAlive.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty())) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле игрока и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void LeaderMakeDistantAttack(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesCanBeAttackedDistantly;
    HeroesCanBeAttackedDistantly.resize(0);
    Position *leader = pole_.GetPosition(1, 1, friendlyside);
    heroes.push_back(&leader->GetHero());
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty())) {
            HeroesCanBeAttackedDistantly.push_back(true);
        } else {
            HeroesCanBeAttackedDistantly.push_back(false);
        }
    }
    //return HeroesAlive;
    //GetCoordinates
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside ? 0 : 1);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().frontLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле противника, у которых CanBeAttackedDistantly вернло true и кол-во, которое он может выбрать - 1
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->frontLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)

    //Исправить: добавить в массив ЖИВЫХ
}

void HitAvangard(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < 3; i++) {
        Position *chosen_pos = pole_.GetPosition(i, 0, friendlyside ? 0 : 1);
        heroes.push_back(&(chosen_pos->GetHero()));
    }
    if (heroes.size() > 1)
        position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1-4)персонажей из авангарда противника
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void PlusPowerByDead(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetSide() == friendlyside)) {
            Position *chosen_pos = pole_.GetPosition(pole_.GetVector()[i]->GetCell(), pole_.GetVector()[i]->GetLine(),
                                                     friendlyside);
            heroes.push_back(&(chosen_pos->GetHero()));
        }
    }
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)всех мертвых в отряде игрока
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void FrontAndBackGetDamage(Position *position, Pole &pole_, int friendlyside) {
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

    if (heroes.size() > 1)
        position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)позади стоящего 2)впереди стоящего
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void PlusPowerInCloseAttack(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAliveEnemies;
    HeroesAliveEnemies.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() != friendlyside)) {
            HeroesAliveEnemies.push_back(true);
        } else {
            HeroesAliveEnemies.push_back(false);
        }
    }
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле противника
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)

}

void ResAndGetDamage(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetSide() == friendlyside)) {
            Position *chosen_pos = pole_.GetPosition(pole_.GetVector()[i]->GetCell(), pole_.GetVector()[i]->GetLine(),
                                                     friendlyside);
            heroes.push_back(&(position->GetHero()));
        }
    }
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 мертвые персонажи в отряде игрока
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void KillWthDamage(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAliveEnemies;
    HeroesAliveEnemies.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() != friendlyside)) {
            HeroesAliveEnemies.push_back(true);
        } else {
            HeroesAliveEnemies.push_back(false);
        }
    }
    int xCoord = 0;
    int yCoord = 0;
    std::cin >> xCoord >> yCoord;
    Position *chosen_pos = pole_.GetPosition(xCoord, yCoord, friendlyside);
    heroes.push_back(&(chosen_pos->GetHero()));
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле противника
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
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
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле противника, у которых CanBeAttackedFromDistant вернул true
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void CastlingWithDead(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if ((pole_.GetVector()[i]->GetHero().IsDead()) && (pole_.GetVector()[i]->GetSide() == friendlyside)) {
            Position *chosen_pos = pole_.GetPosition(pole_.GetVector()[i]->GetCell(), pole_.GetVector()[i]->GetLine(),
                                                     friendlyside);
            heroes.push_back(&(position->GetHero()));
        }
    }
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 мертвые персонажи игрока
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)персонажа, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)

}

void DamageTwoRows(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> HeroesAliveEnemies;
    HeroesAliveEnemies.resize(0);
    int count = 0;
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() != friendlyside)) {
            HeroesAliveEnemies.push_back(true);
        } else {
            HeroesAliveEnemies.push_back(false);
        }
    }
    for (int i = 0; i < pole_.GetVector().size(); i++) {
        if (!(pole_.GetVector()[i]->isEmpty()) && (!(pole_.GetVector()[i]->GetHero().IsDead())) &&
            (pole_.GetVector()[i]->GetLine() == position->GetLine())) {
            heroes.push_back(&(pole_.GetVector()[i]->GetHero()));
        }
    }
    position->GetHero().middleLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //отправить Степе массив bool, среди которых 1 живые персонажи на поле противника
    //СТепа вернет координаты выбранного персонажа
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем
    // 1-7)персонажи, которые стоят в той же линии, где персонаж, которого выбрал Степа
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)

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
            (!pole_.GetVector()[i]->isEmpty() && (pole_.GetVector()[i]->GetSide() != friendlyside))) {
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
        if (!(pole_.GetVector()[i]->GetHero().IsDead()) && (!pole_.GetVector()[i]->isEmpty()) &&
            (pole_.GetVector()[i]->GetSide() == friendlyside)) {
            HeroesAlive.push_back(true);
        } else {
            HeroesAlive.push_back(false);
        }
    }
    return HeroesAlive;
}

vector<bool> SendAllDeadFriends(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
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
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1)лидера противника
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}

void HitFrontRowHeroes(Position *position, Pole &pole_, int friendlyside) {
    std::vector<Hero *> heroes;
    heroes.resize(0);
    heroes.push_back(&(position->GetHero()));
    std::vector<bool> ToAnalyse;
    int enemyside = friendlyside ? 0 : 1;
//    ToAnalyse = AttackRequest(pole_, enemyside);
     for (int i = (9 * enemyside), j = 0; i < (9 * enemyside) + 9; i++, j++) {
         if (ToAnalyse[i] == true) {
             heroes.push_back(&(pole_.GetVector()[i]->GetHero()));
         }
     }
    position->GetHero().backLinePower(heroes);
//    std::vector<Breed> request = ReturnRequest(pole_);
    //в вектор vector<Hero*> heroes добавить 0)персонажа, чью спсобность используем 1-4)всех героев в ближнем бою
    //персонаж_чью_способность_используем->middleLinePower(heroes);
    //Степе отправляется массив из 18 полей (в каждом поле хп и сила героя)
}
