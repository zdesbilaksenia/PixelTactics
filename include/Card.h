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
    bool mouseIsPressed();
    void draw();
    bool takeCard();
    void updateHand();
    void setCardShirtTexture(Texture *_texture);
    void setStatus(CardsManagerStatus _status);
    void setUnitBuffer(Unit &_unit);
    bool canTakeCard();

    ~CardsManager();

private:
    //Unit *unitBuffer;
    unique_ptr<Unit> unitBuffer;
    RenderWindow &window;
    CardsManagerStatus status;
    //Заменить на умные указатели!
    vector<Card *> cardsInHand;
    stack<Card *> cardsInStack;

    vector<Card *>::iterator cardToDeleteId;

    //В буфер tilesManager будем складывать юнита
    TilesManager &tilesManager;
    //unique_ptr

    RectangleShape cardShirtRect;

    const Color &colorReleasingCard = Color::Green;
};