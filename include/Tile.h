#pragma once

//#include "DrawableBox.h"
#include "Clickable.h"
#include "Unit.h"
#include <vector>
#include <memory>
using std::make_unique;
using std::unique_ptr;
using std::vector;

enum class TileStatus
{
    statusIsEmpty,
    statusHasDeadBody,
    statusHasUnit
};

class Tile : public Clickable
//class Tile : public DrawableBox
{
public:
    Tile(RenderWindow &_window, Mouse &_mouse);
    //Tile(RenderWindow& _window);
    void setUnit(Unit *_unit);
    void setStatus(TileStatus _status);
    void draw() override;
    TileStatus getStatus();
    ~Tile();

private:
    Unit *unit;
    TileStatus status;
};

//Например, игрок нажал на кнопку "Аттаковать"
//Сама комманда не знает, кого он аттакует, и она не может "ждать"
//Сохраняем статус комманды в enum и уже в цикле обработки сообщений будем от него отталкиваться.
//
enum class TilesManagerStatus
{
    //Когда ничего не происходит
    statusNothingHappens,
    //Готовы выложить карту
    statusReleasingUnit,
    //Готовы атаковать юнита противника
    statusAttackingUnit,
    //Только что реализовали карту, статус нужен для CardManager
    statusCardWasJustReleased
};

enum class Side
{
    sidePlayer,
    sideOpponent
};

class TilesManager
{
public:
    TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side);

    void setStatus(const TilesManagerStatus &_status);
    void setUnitBuffer(Unit *_unit);
    Unit *getUnitBuffer();
    void mouseIsPressed();
    void updateFocus();
    void draw();
    void setTexture(Texture *_texture);
    bool hasEmptyTiles();
    TilesManagerStatus getStatus();

    ~TilesManager();

private:
    Side side;
    vector<Tile *> tiles;
    //Тайлы авангарда
    vector<unique_ptr<Tile>> tilesAvangard;
    //Тайлы фланга
    vector<unique_ptr<Tile>> tilesFlank;
    //Тайлы тыла
    vector<unique_ptr<Tile>> tilesRear;

    //Когда завершилось какое-то действие (разыграна карта или атакован игрок)
    void setNormalColors();

    //Какую карту мы хотим вывести в игру
    //Указатель или ссылка?
    Unit *unitBuffer;

    //Статус
    TilesManagerStatus status;

    //Цвета для всех случаев
    const Color colorBasic = Color::White;
    const Color colorInFocus = Color(65, 105, 225);
    const Color colorForReleasingUnit = Color::Yellow;
    const Color colorWhenCantReleaseUnit = Color::Black;
    const Color colorForDeadBody = Color::Magenta;
    const Color colorToBeAttacked = Color::Red;
};