#include <iostream>
#include <vector>
#include "Card.h"
#include "Position.h"
#include "Deck.h"

class Player{
public:
    Player();
    Player(const Player& other);
    Player& operator = (const Player& other);
    ~Player();

    //Выбрать карту из руки 
    Card& ChooseCard();
    //Сыграть карту
    void PlayCard(Card& card);
    //Взять карту из колоды
    void DrawCard();
    //Аттаковать своим героем
    void Attack(Position friendlyHero);
    //Убрать тело с поля
    void RemoveDead(Position Corpse);
    //Закончить ход
    void EndTurn();
    //Узнать позицию лидера
    Position GetLeaderPosition();

private:
    //Рука Карт
    std::vector <Card> hand;
    //Макс. Размер руки
    unsigned int maxHandSize;
    //Действия на ход (У каждого игрока по 2)
    unsigned int movesAmount;
    //Колода игрока
    Deck deck;
    //Позиция лидера
    Position leader;

};