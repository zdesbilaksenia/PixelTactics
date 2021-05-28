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
    if (_unit == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "Card::setUnit() : _unit is nullptr!";
        return;
    }
    this->unit = _unit;
}

void Card::click()
{
    this->status = CardStatus::statusCardWasReleased;
    this->command->execute();
}

void Card::updateFocus()
{
    //Clickable::updateFocus();
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
    unit->draw(mouse);
}

Card::~Card()
{
    unit = nullptr;
}

//=====================================
//===========CardsManager==============
//=====================================

CardsManager::CardsManager(RenderWindow &_window, TilesManager &_tilesManager, stack<Card *> _cardsInStack) : window(_window),
                                                                                                              tilesManager(_tilesManager),
                                                                                                              cardsInStack(_cardsInStack)
{
    status = CardsManagerStatus::statusNothingHappens;
    cardShirtRect.setSize(Vector2f(cardWidth, cardHeight));
}

bool CardsManager::takeCard()
{
    if (canTakeCard())
    {
        cardWasTaken = true;
        cardsInHand.push_back(cardsInStack.top());
        cardsInStack.pop();
        this->updateHand();
        return true;
    }
    return false;
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

void CardsManager::updateFocus()
{
    for (auto cardId = cardsInHand.rbegin(); cardId != cardsInHand.rend(); ++cardId)
    {
        (*cardId)->updateFocus();
    }
}

bool CardsManager::mouseIsPressed()
{
    switch (status)
    {
    case CardsManagerStatus::statusGameStarting:
    {
        if (tilesManager.getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusGameStarting Card was just released!";
            return true;
        }
        for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                cardToDeleteId = cardId;
                //Верхняя строка - авангард
                //Этот массив получаем от сервера!!!
                vector<bool> activeTiles =
                    {0, 0, 0,
                     0, 1, 0,
                     0, 0, 0};
                tilesManager.setActiveTiles(activeTiles);
                tilesManager.setUnitBuffer(*(*cardId)->unit);
                BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusGameStarting Card was clicked!";
                return true;
            }
        }
        break;
    }

    case CardsManagerStatus::statusNothingHappens:
    {
        //Если некуда ставить
        if (tilesManager.hasEmptyTiles())
        {
            for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
            {
                if ((*cardId)->hasFocus())
                {
                    BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusNothingHappens Card was clicked!";
                    cardToDeleteId = cardId;
                    tilesManager.setUnitBuffer(*(*cardId)->unit);
                    return true;
                }
            }
        }
        return false;
        break;
    }

    case CardsManagerStatus::statusReleasingCard:
    {
        //Если решили взять другую карту
        for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                cardToDeleteId = cardId;
                tilesManager.setUnitBuffer(*(*cardId)->unit);
                tilesManager.setStatus(TilesManagerStatus::statusReleasingCard);
                status = CardsManagerStatus::statusReleasingCard;
                tilesManager.updateFocus();
            }
        }

        /*
        if (tilesManager.getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            tilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
            BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusReleasingCard card was just released, removing card from hand!";
            cardsInHand.erase(cardToDeleteId);
            this->updateHand();
            status = CardsManagerStatus::statusNothingHappens;
        }
        */
        removeSelectedCard();

        return false;
        break;
    }

    case CardsManagerStatus::statusGameStartingReleasingCard:
    {
        //Если решили взять другую карту
        for (auto cardId = cardsInHand.begin(); cardId != cardsInHand.end(); ++cardId)
        {
            if ((*cardId)->hasFocus())
            {
                cardToDeleteId = cardId;
                tilesManager.setUnitBuffer(*(*cardId)->unit);
                tilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
                status = CardsManagerStatus::statusGameStartingReleasingCard;
                tilesManager.updateFocus();
            }
        }

        /*
        if (tilesManager.getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            tilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
            BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusGameStartingReleasingCard card was just released!";
            cardsInHand.erase(cardToDeleteId);
            this->updateHand();
            status = CardsManagerStatus::statusNothingHappens;
        }
        */

        removeSelectedCard();

        return false;
        break;
    }
    }
    return false;
}

void CardsManager::draw()
{
    for (size_t i = 0; i < cardsInStack.size(); ++i)
    {
        cardShirtRect.setPosition(Vector2f(cardsInStackPosX + 4 * i, cardsInStackPosY));
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

bool CardsManager::canTakeCard()
{
    return (cardsInHand.size() < maxNumberOfCardsInHand && cardsInStack.size() > 0);
}

void CardsManager::setStatus(CardsManagerStatus _status)
{
    status = _status;
}

void CardsManager::setUnitBuffer(Unit *unit)
{
    unitBuffer = unit;
}

bool CardsManager::getCardWasTaken()
{
    bool result = cardWasTaken;
    cardWasTaken = false;
    return result;
}

CardsManager::~CardsManager() {}