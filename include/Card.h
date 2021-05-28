#pragma once

#include "Clickable.h"
#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include <boost/log/trivial.hpp>
#include <vector>
#include <stack>
using std::stack;
using std::unique_ptr;
using std::vector;
using namespace boost::log;

enum class CardStatus
{
    statusNothingHappens,
    statusReleasingCard,
    statusCardWasReleased
};

class Card : public Clickable
{
public:
    Card(RenderWindow &_window, Mouse &_mouse);
    void setUnit(Unit *_unit);
    void draw() override;
    void click() override;
    void updateFocus() override;
    void setStatus(const CardStatus &_status);
    void setPosition(const int &_posX, const int &_posY) override;

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
    //Начало игры
    statusGameStarting,
    //Начало игры, реализуем главного героя
    statusGameStartingReleasingCard
};

class CardsManager
{
public:
    CardsManager(RenderWindow &_window, TilesManager &_tilesManager, stack<Card *> _cardsInStack);

    void updateFocus();
    void draw();
    bool takeCard();
    void updateHand();
    void setCardShirtTexture(Texture *_texture);
    void setStatus(CardsManagerStatus _status);
    void setUnitBuffer(Unit *unit);
    bool canTakeCard();
    bool getCardWasTaken();
    int numberOfCardsInHand()
    {
        return cardsInHand.size();
    }

    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    void removeSelectedCard()
    {
        if (tilesManager.getStatus() == TilesManagerStatus::statusCardWasJustReleased)
        {
            tilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
            BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseIsPressed() : statusReleasingCard card was just released, removing card from hand!";
            cardsInHand.erase(cardToDeleteId);
            this->updateHand();
            status = CardsManagerStatus::statusNothingHappens;
        }
    }

    void handleClick(Card &card)
    {
        switch (status)
        {
        case CardsManagerStatus::statusGameStarting:
        {
            BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusGameStarting!";
            vector<bool> activeTiles =
                {0, 0, 0,
                 0, 1, 0,
                 0, 0, 0};
            tilesManager.setActiveTiles(activeTiles);
            (*cardToDeleteId)->setFillColor(colorReleasingCard);
            tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
            tilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
            BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusGameStarting Card was clicked!";
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
        case CardsManagerStatus::statusGameStartingReleasingCard:
        {
            BOOST_LOG_TRIVIAL(info) << "CardsManager::handleClick() : statusGameStartingReleasingCard!";

            tilesManager.setUnitBuffer(*(*cardToDeleteId)->unit);
            tilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
            break;
        }
        }
        return;
    }

    //Аналог mouseIsPressed().
    //Надо сделать setActiveTiles для авангарда, фланга и тыла
    void mouseClicked()
    {
        BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseClicked() : start!";
        removeSelectedCard();
        for (auto card = cardsInHand.begin(); card != cardsInHand.end(); card++)
        {
            if ((*card)->hasFocus())
            {
                cardToDeleteId = card;
                handleClick(*(*card));
                BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseClicked() : end!";
                return;
            }
        }
        BOOST_LOG_TRIVIAL(info) << "CardsManager::mouseClicked() : end, no card was chosen!";
    }
    //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

    ~CardsManager();

private:
    bool cardWasTaken;
    Unit *unitBuffer;
    RenderWindow &window;
    CardsManagerStatus status;
    //Заменить на умные указатели!
    vector<Card *> cardsInHand;
    stack<Card *> cardsInStack;

    vector<Card *>::iterator cardToDeleteId;

    //В буфер tilesManager будем складывать юнита
    TilesManager &tilesManager;

    RectangleShape cardShirtRect;

    const Color &colorReleasingCard = Color::Green;
};