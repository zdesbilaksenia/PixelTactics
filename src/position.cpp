#include "../lib/position.h"




void Position::SetHero(Card card){
    Hero hero(card);
    hero_ = hero;
    empty_ = false;
}

int Position::GetCell(){return cell_;}
int Position::GetLine(){return line_;}
int Position::GetSide(){return side_;}
bool Position::isEmpty(){return empty_;}
Hero* Position::GetHero(){return &hero_;}


void Position::InfoPosition(){
    if(!(*this).isEmpty()){
        std::cout << "Здоровье Героя:" << (*this).GetHero()->GetCurHealth() << " " << "Атака Героя:" << (*this).GetHero()->GetCurStrength() << std::endl;
    }
}

void Position::RemoveHero(){
        empty_ = true;
    }
    