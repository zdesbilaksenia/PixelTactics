#pragma once
#ifndef DECK_H
#define DECK_H

#include "card.h"
#include <vector>
#include <boost/random.hpp>
#include <iostream>
#include <ctime>
#include <cstdint>
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>


class Deck {
public:
    Deck();

    Deck(std::vector<Card> &Cards);

    int roll_card(int count);

    void ShowDeck();

    void push_back(Card card);

    std::vector<Card> &GetVector();

    Card ChooseCard(int cardIndex);

private:
    std::vector<Card> deck_;
};

#endif