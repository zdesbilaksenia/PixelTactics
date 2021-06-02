#include "Card.h"
#include "configurations.cpp"
#include <iostream>
using std::cout;
using std::endl;

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

void Card::setUnit(Unit& _unit)
{
    this->unit = &_unit;
}

void Card::click()
{
    this->status = CardStatus::statusCardWasReleased;
    this->command->execute();
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

void Card::updateFocus()
{
    if (this->hasFocus())
    {
        DrawableBox::setPosition(defaultPosX, defaultPosY - 20);
    }
    else
    {
        DrawableBox::setPosition(defaultPosX, defaultPosY);
    }
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

void CardsManager::setCardShirtTexture(Texture *_tx)
{
    cardShirtRect.setTexture(_tx);
}

void CardsManager::mouseClicked()
{
    removeSelectedCard();
    if (status == CardsManagerStatus::statusReleasingCard || status == CardsManagerStatus::statusGameStartingReleasingCard)
    {
        (*cardToDeleteId)->setFillColor(Color::White);
    }
    for (auto card = cardsInHand.begin(); card != cardsInHand.end(); card++)
    {
        if ((*card)->hasFocus())
        {
            cardToDeleteId = card;
            handleClick(*(*card));
            return;
        }
    }
}

void CardsManager::handleClick(Card &card)
{
    switch (status)
    {
    case CardsManagerStatus::statusGameStarting:
    {
        BOOST_LOG_TRIVIAL(debug) << "CardsManager::handleClick() : statusGameStarting!";
        vector<bool> activeTiles =
            {0, 0, 0,
             0, 1, 0,
             0, 0, 0};
        tilesManager.setActiveTiles(activeTiles);
        (*cardToDeleteId)->setFillColor(colorReleasingCard);
        tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
        tilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
        this->setStatus(CardsManagerStatus::statusGameStartingReleasingCard);
        BOOST_LOG_TRIVIAL(debug) << "CardsManager::handleClick() : statusGameStarting Card was clicked!";
        break;
    }
    case CardsManagerStatus::statusGameStartingReleasingCard:
    {
        BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusGameStartingReleasingCard!";
        (*cardToDeleteId)->setFillColor(colorReleasingCard);
        tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
        tilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
        break;
    }
    case CardsManagerStatus::statusNothingHappens:
    {
        BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusNothingHappens!";
        if (tilesManager.hasEmptyTiles())
        {
            BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusNothingHappens Card was clicked!";
            (*cardToDeleteId)->setFillColor(colorReleasingCard);
            tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
            tilesManager.setStatus(TilesManagerStatus::statusReleasingCard);
            status = CardsManagerStatus::statusReleasingCard;
        }
        break;
    }
    case CardsManagerStatus::statusReleasingCard:
    {
        BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusReleasingCard!";
        (*cardToDeleteId)->setFillColor(colorReleasingCard);
        tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
        tilesManager.setStatus(TilesManagerStatus::statusReleasingCard);
        break;
    }
    }
    return;
}

void CardsManager::updateFocus()
{
    for (auto cardId = cardsInHand.rbegin(); cardId != cardsInHand.rend(); ++cardId)
    {
        (*cardId)->updateFocus();
    }
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

void CardsManager::setStatus(const CardsManagerStatus &_status)
{
    status = _status;
}

int CardsManager::numberOfCardsInHand()
{
    return cardsInHand.size();
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

bool CardsManager::canTakeCard()
{
    return (cardsInHand.size() < maxNumberOfCardsInHand && cardsInStack.size() > 0);
}

bool CardsManager::getCardWasTaken()
{
    bool result = cardWasTaken;
    cardWasTaken = false;
    return result;
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

void CardsManager::removeSelectedCard()
{
    if (tilesManager.getStatus() == TilesManagerStatus::statusCardWasJustReleased)
    {
        tilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
        BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusReleasingCard card was just released, removing card from hand!";
        cardsInHand.erase(cardToDeleteId);
        this->updateHand();
        tilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
        status = CardsManagerStatus::statusNothingHappens;
    }
}

CardsManager::~CardsManager()
{
    unitBuffer = nullptr;
}