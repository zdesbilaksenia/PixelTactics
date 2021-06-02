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
    //Мб unit сразу загрузить в конструктор засунуть, не помню, почему так не сделал
    Card(RenderWindow &_window, Mouse &_mouse);
    void setPosition(const int &_posX, const int &_posY) override;
    void setUnit(Unit& _unit);
    void click() override;
    void draw() override;
    void updateFocus() override;
    Unit *unit;

    ~Card();

private:
    CardStatus status;
    int defaultPosX;
    int defaultPosY;
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

    void setCardShirtTexture(Texture *_texture);
    void mouseClicked();
    void handleClick(Card &card);
    void updateFocus();
    void draw();
    void setStatus(const CardsManagerStatus &_status);
    int numberOfCardsInHand();
    bool takeCard();
    bool canTakeCard();
    bool getCardWasTaken();

    ~CardsManager();

private:
    RenderWindow &window;
    //В буфер tilesManager будем складывать юнита
    TilesManager &tilesManager;
    stack<Card *> cardsInStack;
    vector<Card *> cardsInHand;
    Unit *unitBuffer;
    vector<Card *>::iterator cardToDeleteId;
    CardsManagerStatus status;
    bool cardWasTaken;
    RectangleShape cardShirtRect;
    const Color &colorReleasingCard = Color::Green;

    void updateHand();
    void removeSelectedCard();
};