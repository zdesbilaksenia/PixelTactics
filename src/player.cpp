#include "player.h"
#include <iostream>

Player::Player(Deck& deck,int side):deck_(deck),side_(side){}

Deck& Player::GetDeck(){
    return deck_;
}

Deck& Player::GetHand(){
    return hand_;
}

void Player::StartingHand(){
    for(int i = 0; i < 4; i++){
        DrawCard();
    }
}

void Player::DrawCard(){
    int size = deck_.GetVector().size();
    hand_.push_back(deck_.GetVector()[size-1]);
    deck_.GetVector().pop_back();
}

Card Player::ChooseCard(int CardIndex){
    int size = deck_.GetVector().size();
    if(CardIndex < 0 || CardIndex >= size){
        //throw exception
    }
    Card card = hand_.GetVector()[CardIndex];
    hand_.GetVector().erase(hand_.GetVector().begin()+CardIndex);
    return card;
}

int Player::GetSide(){
    return side_;
}

void Player::Attack(Position& FriendlyHero, Position& EnemyHero){
    Hero* Friend = (FriendlyHero.GetHero());
    Hero* Enemy = (EnemyHero.GetHero());
    Friend->Attack(*Enemy,Friend->GetCurStrength());
}
//Убрать 
bool Player::MeleeAttackCheck(Position& EnemyHero, Pole& pole){
    if(EnemyHero.GetLine() == 0) return true;
    if(EnemyHero.GetLine() == 1){
        if(pole.GetPosition(EnemyHero.GetCell(),0,EnemyHero.GetSide()).isEmpty()) return true;
        else return false;
    }
    if(EnemyHero.GetLine() == 2){
        if((pole.GetPosition(EnemyHero.GetCell(),0,EnemyHero.GetSide()).isEmpty()) && 
            (pole.GetPosition(EnemyHero.GetCell(),1,EnemyHero.GetSide()).isEmpty())) return true;
        else return false;
    }
}



/*Position& Player::ChooseCoordinates(int cell, int line, Pole pole){
    Position position;
    position = pole.GetPosition(cell,line,side_);
    if(position.isEmpty()) return position;
    Position errorPosition;
    return errorPosition;
}*/

/*void Player::PlayCard(Pole pole){
    int CardIndex = 0;
    std::cout << "Введите номер карты с 0" << std::endl;
    std::cin >> CardIndex;
    Card* card = ChooseCard(CardIndex);
    int cell = 0;
    int line = 0;
    std::cout << "Введите клетку и линию с 0" << std::endl;
    std::cin >> cell >> line;
    Position p_position = ChooseCoordinates(cell,line,pole);
    p_position.SetHero(card);
}
*/