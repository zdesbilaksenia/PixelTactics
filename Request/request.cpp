#include "request.h"
#include "pole.h"

vector<bool> AttackRequest(Pole& pole_,int& side){
    return pole_.CanBeMeleeAttackedRequest(side);
}

int AttackWasDone(int xAttack, int yAttack, int xAttacked, int yAttacked, int friendlyside, Pole& pole_){
    Position friendPosition;
    friendPosition = pole_.GetPosition(xAttack,yAttack,friendlyside);
    Position enemyPosition;
    enemyPosition = pole_.GetPosition(xAttacked,yAttacked,friendlyside ? 0 : 1);
    friendPosition.GetHero().Attack(enemyPosition.GetHero(), enemyPosition.GetHero().GetCurStrength());
    pole_.SetPosition(enemyPosition);
}