#include "pole.h"
#include "vector"


using std::vector;

struct Breed{
    int HP;
    int Strength;
};

vector<Breed> ReturnRequest(Pole& pole_);

vector<bool> AttackRequest(Pole& pole_,int& side);

int AttackWasDone(int xAttack, int yAttack, int xAttacked, int yAttacked, int friendlyside, Pole& pole_);

void UseMagic(int xAttack, int yAttack, int friendlyside, Pole& pole_,int phase);

void FrontRequest(Position* friendPosition,Pole& pole,int friendlyside);

void MiddleRequest(Position* friendPosition,Pole& pole, int friendlyside);

void BackRequest(int ID);

void Interception(Position* position,Pole& pole_);