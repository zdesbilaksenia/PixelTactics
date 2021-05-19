#include "hero.h"


Hero::Hero():health_(0),strength_(0){}

Hero::Hero(Card* card){
    health_ = card->GetBreed().GetHealth();
    strength_ = card->GetBreed().GetStrength();
}

/*Hero& Hero::operator=(const Hero& hero){
    health_ = hero.health_;
    strength_ = hero.strength_;
}*/