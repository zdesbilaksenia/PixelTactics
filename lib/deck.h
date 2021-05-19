#pragma once
#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>


class Deck{
public:
    Deck();
    Deck(std::vector<Card*>& Cards);
    void ShowDeck();
    void push_back(Card* card);
    std::vector <Card*>& GetVector();

private:
    std::vector<Card*> deck_;
};
#endif