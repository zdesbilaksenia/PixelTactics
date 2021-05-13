#pragma once

#include "Clickable.h"
#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include <vector>
using std::vector;

enum class CardStatus
{
    statusNothingHappens,
    statusReleasingCard,
    statusCardWasReleased
};

class Card : public Clickable
{
public:
    Card(RenderWindow &_window, Mouse &_mouse, Command *_command);
    void setUnit(Unit *_unit);
    void draw() override;
    void click() override;
    void updateFocus() override;
    void setStatus(const CardStatus &_status);
    void setPosition(const int& _posX, const int& _posY) override;

    ~Card();

    //Потом в private как-то занести надо
    Unit *unit;

private:
    CardStatus status;
    int defaultPosX;
    int defaultPosY;
    //Label healthLabel;
    //Label attackLabel;
};

enum class CardsManagerStatus
{
    //Ничего не происходит
    statusNothingHappens,
    //Кладём карту
    statusReleasingCard,
};

class CardsManager
{
public:
    CardsManager();

    //Сделать потом константную ссылку
    void setCards(vector<Card *> _cards);
    void setTilesManager(TilesManager *_tileManager);
    void updateFocus();
    void mouseIsPressed();
    void draw();

    ~CardsManager();

private:
    CardsManagerStatus status;
    vector<Card *> cards;
    vector<Card* >::iterator cardToDeleteId;

    //В буфер tilesManager будем складывать юнита
    TilesManager *tilesManager;

    int numberOfCards;
};