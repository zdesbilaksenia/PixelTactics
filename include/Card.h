#pragma once

#include "Clickable.h"
#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include <vector>
using std::vector;

enum CardStatus 
{
    statusReleasingCard
};

class Card : public Clickable
{
public:

    Card(RenderWindow& _window, Mouse& _mouse, Command* _command);
    void setUnit(Unit* _unit);
    void draw() override;
    void click() override;

    ~Card();

private:

    Unit* unit;
    //Label healthLabel;
    //Label attackLabel;
};

class CardsManager
{
public:
    CardsManager();

    //Сделать потом константную ссылку
   void setCards(vector<Card*> _cards);
   void setTilesManager(TilesManager* _tileManager);
   void updateFocus();
   void mouseIsPressed();
   void draw();

   ~CardsManager();


private:

    vector<Card*> cards;

    //В буфер tilesManager будем складывать юнита
    TilesManager* tilesManager;

    int numberOfCards;

};