#include "Card.h"

#include <iostream>
using namespace std;

Card::Card(RenderWindow &_window, Mouse &_mouse, Command *_command) : Clickable(_window, _mouse, _command) {}

void Card::setPosition(const int& _posX, const int& _posY)
{
    DrawableBox::setPosition(_posX, _posY);
    defaultPosX = _posX;
    defaultPosY = _posY;
}

void Card::setUnit(Unit *_unit)
{
    this->unit = _unit;
}

void Card::click()
{
    this->status = CardStatus::statusCardWasReleased;
    this->command->execute();
    cout << unit << endl;
    //this->unit = nullptr;
}

void Card::updateFocus()
{
    Clickable::updateFocus();
    if(this->hasFocus())
    {
        DrawableBox::setPosition(defaultPosX, defaultPosY - 20);
    }
    else 
    {
        DrawableBox::setPosition(defaultPosX, defaultPosY);
    }
}

void Card::draw()
{
    window.draw(rect);

    if (status == CardStatus::statusCardWasReleased)
    {
        return;
    }
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
    status = CardsManagerStatus::statusNothingHappens;
    cardToDeleteId = cards.begin();
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
    switch (status)
    {
    case CardsManagerStatus::statusNothingHappens:
        for (auto cardId = cards.begin(); cardId != cards.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                (*cardId)->click();
                cardToDeleteId = cardId;
                tilesManager->setUnitBuffer((*cardId)->unit);
                //this->tilesManager->setStatus(TilesManagerStatus::statusReleasingUnit);
                status = CardsManagerStatus::statusReleasingCard;

                ////////////////////////////////////////////////////////////////////////////////////////COUT
                cout << (*cardId)->unit << endl;
            }
        }
        break;
    case CardsManagerStatus::statusReleasingCard:

        if (tilesManager->getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "CARD WAS JUST RELEASED!!!" << endl;

            cards.erase(cardToDeleteId);

            cout<<cards.size()<<endl;
            status = CardsManagerStatus::statusNothingHappens;
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