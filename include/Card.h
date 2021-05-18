#pragma once

#include "Clickable.h"
#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include <vector>
#include <stack>
using std::stack;
using std::unique_ptr;
using std::vector;

const int maxNumberOfCardsInHand = 7;

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
};

class CardsManager
{
public:
    CardsManager(RenderWindow& _window, stack<Card *> _cardsInStack);

    //Сделать потом константную ссылку
    void setTilesManager(TilesManager *_tileManager);
    void updateFocus();
    void mouseIsPressed();
    void draw();
    bool takeCard();
    void updateHand();
    void setCardShirtTexture(Texture* _texture);

    ~CardsManager();

private:
    RenderWindow& window;
    CardsManagerStatus status;
    //Заменить на умные указатели!
    vector<Card *> cardsInHand;
    stack<Card *> cardsInStack;

    vector<Card *>::iterator cardToDeleteId;

    //В буфер tilesManager будем складывать юнита
    TilesManager *tilesManager;
    //unique_ptr

    RectangleShape cardShirtRect;

    const Color &colorReleasingCard = Color::Green;
};