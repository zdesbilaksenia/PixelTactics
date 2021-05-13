#include "position.h"

Position::Position():cell_(0),line_(0),side_(0),empty_(true),hero_(Hero()){};
Position::Position(int cell, int line, int side):cell_(cell),line_(line),side_(side),empty_(true),hero_(Hero()){};


void Position::SetHero(Card* card){
    Hero hero(card);
    hero_ = hero;
    empty_ = false;
}

int Position::GetCell(){return cell_;}
int Position::GetLine(){return line_;}
int Position::GetSide(){return side_;}
bool Position::isEmpty(){return empty_;}
Hero& Position::GetHero(){return hero_;}


void Position::InfoPosition(){
    if(!(*this).isEmpty()){
        std::cout << "Здоровье Героя:" << (*this).GetHero().GetHealth() << " " << "Атака Героя:" << (*this).GetHero().GetStrength() << std::endl; 
    }
}

void Position::RemoveHero(){
        empty_ = true;
    }