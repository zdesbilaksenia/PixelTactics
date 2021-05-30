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
    void setUnit(Unit *_unit);
    void draw() override;
    void click() override;
    void updateFocus() override;
    void setStatus(const CardStatus &_status);
    void setPosition(const int &_posX, const int &_posY) override;
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

    void updateFocus();
    void draw();
    void updateHand();
    void setCardShirtTexture(Texture *_texture);
    void setStatus(CardsManagerStatus _status);
    void setUnitBuffer(Unit *unit);
    void removeSelectedCard();
    void handleClick(Card &card);
    void mouseClicked();
    bool takeCard();
    bool canTakeCard();
    bool getCardWasTaken();
    int numberOfCardsInHand();

    ~CardsManager();

private:
    bool cardWasTaken;
    Unit *unitBuffer;
    RenderWindow &window;
    CardsManagerStatus status;
    vector<Card *> cardsInHand;
    stack<Card *> cardsInStack;
    vector<Card *>::iterator cardToDeleteId;

    //В буфер tilesManager будем складывать юнита
    TilesManager &tilesManager;
    RectangleShape cardShirtRect;
    const Color &colorReleasingCard = Color::Green;
};