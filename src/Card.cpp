#include "Card.h"
#include "configurations.cpp"
#include <iostream>
using namespace std;

Card::Card(RenderWindow &_window, Mouse &_mouse) : Clickable(_window, _mouse)
{
    this->setSize(cardWidth, cardHeight);
}

void Card::setPosition(const int &_posX, const int &_posY)
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

    cout << "Card::click:: before command execution " << unit << endl;

    this->status = CardStatus::statusCardWasReleased;
    this->command->execute();

    ////////////////////////////////////////////////////////////////COUT
    cout << "Card::click:: after command execution " << unit << endl;
    //this->unit = nullptr;
}

void Card::updateFocus()
{
    Clickable::updateFocus();
    if (this->hasFocus())
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

CardsManager::CardsManager(RenderWindow &_window, stack<Card *> _cardsInStack) : window(_window), cardsInStack(_cardsInStack)
{
    status = CardsManagerStatus::statusNothingHappens;
    cardShirtRect.setSize(Vector2f(cardWidth, cardHeight));
}

bool CardsManager::takeCard()
{
    if (cardsInStack.size() > 0 && cardsInHand.size() < maxNumberOfCardsInHand)
    {
        cardsInHand.push_back(cardsInStack.top());
        cardsInStack.pop();
        this->updateHand();
        return 1;
    }
    return 0;
}

void CardsManager::updateHand()
{
    //Числа исправить
    int i = 0;
    for (auto card : cardsInHand)
    {
        card->setPosition(cardsInHandPosX + i * 100, cardsInHandPosY);
        ++i;
    }
}

void CardsManager::setTilesManager(TilesManager *_tileManager)
{
    this->tilesManager = _tileManager;
}

void CardsManager::updateFocus()
{
    switch (status)
    {
    case CardsManagerStatus::statusNothingHappens:
        for (auto cardId = cardsInHand.rbegin(); cardId != cardsInHand.rend(); ++cardId)
        {
            (*cardId)->updateFocus();
        }
        break;
    case CardsManagerStatus::statusReleasingCard:
        for (auto cardId = cardsInHand.rbegin(); cardId != cardsInHand.rend(); ++cardId)
        {
            (*cardId)->updateFocus();
        }
        (*cardToDeleteId)->setFillColor(colorReleasingCard);
        break;
    default:
        break;
    }
}

void CardsManager::mouseIsPressed()
{
    switch (status)
    {
    case CardsManagerStatus::statusNothingHappens:
    //Если некуда ставить
        if(tilesManager->hasEmptyTiles() == false)
        {
            return;
        }
        for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                cardToDeleteId = cardId;
                tilesManager->setUnitBuffer(*(*cardId)->unit);
                tilesManager->setStatus(TilesManagerStatus::statusReleasingUnit);
                status = CardsManagerStatus::statusReleasingCard;
                return;
            }
        }
        break;
    case CardsManagerStatus::statusReleasingCard:

        //Если решили взять другую карту
        for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                cardToDeleteId = cardId;
                tilesManager->setUnitBuffer(*(*cardId)->unit);
                tilesManager->setStatus(TilesManagerStatus::statusReleasingUnit);
                status = CardsManagerStatus::statusReleasingCard;
                return;
            }
        }

        if (tilesManager->getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            tilesManager->setStatus(TilesManagerStatus::statusNothingHappens);
            ////////////////////////////////////////////////////////////////////////////////////////COUT
            cout << "CardsManager::MouseIsPressed CARD WAS JUST RELEASED!!!" << endl;
            cardsInHand.erase(cardToDeleteId);
            this->updateHand();

            cout << cardsInHand.size() << endl;
            status = CardsManagerStatus::statusNothingHappens;
        }
    }
}

void CardsManager::draw()
{
    for (size_t i = 0; i < cardsInStack.size(); ++i)
    {
        cardShirtRect.setPosition(Vector2f(cardsInStackPosX - 2 * i, cardsInStackPosY));
        window.draw(cardShirtRect);
    }

    for (auto card : cardsInHand)
    {
        card->draw();
    }
}

void CardsManager::setCardShirtTexture(Texture *_tx)
{
    cardShirtRect.setTexture(_tx);
}

CardsManager::~CardsManager()
{
    tilesManager = nullptr;
}