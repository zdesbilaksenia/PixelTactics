#include "deck.h"
#include <ctime>
#include <cstdint>

Deck::Deck(){
    deck_.resize(0);
}

Deck::Deck(std::vector<Card>& Cards){
    int count = Cards.size();
    for(int i = 0; i < 20; i++){
        int random_card = roll_card(count);
        deck_.push_back(Cards[random_card]);
    }
}

void Deck::ShowDeck(){
    for(int i = 0; i < deck_.size(); i++){
        std::cout << deck_[i].name << " " << deck_[i].HP << " " << deck_[i].strength << std::endl;
    }
}

void Deck::push_back(Card card){
    deck_.push_back(card);
}
int Deck::roll_card(int count) {
    return rand()%11;
}

std::vector <Card>& Deck::GetVector(){
    return deck_;
}