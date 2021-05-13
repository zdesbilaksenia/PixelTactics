#include "card.h"

class Card;

Card* Breed::NewCard(){
    return new Card(*this);
}

Breed::Breed(Breed* parent, int health, int strength): health_(health),strength_(strength){
    if (parent != NULL){
        if (health == 0) health_ = parent->GetHealth();
        if (strength == 0) strength_ = parent->GetStrength();
    }
}

int Breed::GetHealth(){
    if(health_ !=0 || parent_ == NULL) return health_;
    return parent_->GetHealth();
}

int Breed::GetStrength(){
    if(strength_ !=0 || parent_ == NULL) return strength_;
    return parent_->GetStrength();
}
Breed& Card::GetBreed(){
    return breed;
}

Card::Card(Breed& breed): health(breed.GetHealth()), strength(breed.GetStrength()), breed(breed){}