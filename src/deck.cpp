#include "deck.h"
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>

boost::random::mt19937 gen;

int roll_card(int count) {
    boost::random::uniform_int_distribution<> dist(0, count-1);
    return dist(gen);
}

Deck::Deck(){
    deck_.resize(0);
}

Deck::Deck(std::vector<Card*>& Cards){
    int count = Cards.size();
    for(int i = 0; i < 20; i++){
        int random_card = roll_card(count);
        deck_.push_back(Cards[random_card]);
    }
}

void Deck::ShowDeck(){
    for(int i = 0; i < deck_.size(); i++){
        std::cout << deck_[i]->GetBreed().GetHealth() << " " << deck_[i]->GetBreed().GetStrength() << std::endl;
    }
}

void Deck::push_back(Card* card){
    deck_.push_back(card);
}

std::vector <Card*>& Deck::GetVector(){
    return deck_;
}