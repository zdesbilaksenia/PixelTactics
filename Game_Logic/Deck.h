#include <iostream>
#include <vector>
#include "Card.h"

class Deck{
public:
    Deck();
    Deck(const Deck& other);
    Deck& operator = (const Deck& other);
    ~Deck();

    //Взять карту из колоды
    Card GetCardFromDeck()
    //Проверка на пустоту колоды
    bool isEmpty();
    //Проверка на кол-во
    bool isMax();

    std::vector <Card> GetDeck();

private:
    //Макс. Размер руки
    unsigned int maxDeckSize;
    //Колода (Массив карт)
    std::vector <Card> deck;
};