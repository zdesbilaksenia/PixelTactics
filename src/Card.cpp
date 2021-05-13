#include "Card.h"

#include <iostream>
using namespace std;

Card::Card(RenderWindow &_window, Mouse &_mouse, Command *_command) : Clickable(_window, _mouse, _command) {}

void Card::setUnit(Unit *_unit)
{
    this->unit = _unit;
}

void Card::click()
{
    this->command->execute();
    //this->unit = nullptr;
}

void Card::draw()
{
    window.draw(rect);

    //Цифры взяты с потолка!
    //Скорее всего, потом надо будет сделать CardManager
    unit->setPosition(rect.getPosition().x + 10, rect.getPosition().y + 10);

    unit->draw();
}

Card::~Card()
{
    unit = nullptr;
}

//=====================================
//===========CardsManager==============
//=====================================

CardsManager::CardsManager()
{
}

void CardsManager::setCards(vector<Card *> _cards)
{
    for (auto _card : _cards)
    {
        cards.push_back(_card);
    }
}

void CardsManager::setTilesManager(TilesManager *_tileManager)
{
    this->tilesManager = _tileManager;
}

void CardsManager::updateFocus()
{
    for (auto card : cards)
    {
        card->updateFocus();
    }
}

void CardsManager::mouseIsPressed()
{
    for (auto card : cards)
    {
        if (card->hasFocus())
        {
            card->click();
        }
    }
}

void CardsManager::draw()
{
    for (auto card : cards)
    {
        card->draw();
    }
}

CardsManager::~CardsManager()
{
    tilesManager = nullptr;
}