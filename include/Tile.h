#pragma once

//#include "DrawableBox.h"
#include "Clickable.h"
#include "Unit.h"
#include "Button.h"
#include <vector>
#include <memory>
using std::make_unique;
using std::unique_ptr;
using std::vector;

enum class TileStatus
{
    statusIsEmpty,
    statusHasDeadBody,
    statusHasUnit,
    statusAttacker
};

class Tile : public Clickable
//class Tile : public DrawableBox
{
public:
    Tile(RenderWindow &_window, Mouse &_mouse);
    //Tile(RenderWindow& _window);
    void setUnit(Unit &_unit);
    void setStatus(TileStatus _status);
    void draw() override;

    void setButtons(Button *_btnAttack, Button *_btnPower)
    {
        buttonAttack = _btnAttack;
        buttonPower = _btnPower;
    };
    Unit *getUnit()
    {
        return unit.get();
    }

    TileStatus getStatus();
    ~Tile();

private:
    Button *buttonAttack;
    Button *buttonPower;

    unique_ptr<Unit> unit;
    //Unit *unit;
    TileStatus status;
    const Color colorForAttack = Color(255, 111, 0);
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
    statusCardWasJustReleased,
    //Когда атакуем тайлы противника
    statusWaitingForAttack,
    //Когда используем силу на тайлы противника
    statusWaitngForPower
};

enum class Side
{
    sidePlayer,
    sideOpponent
};

enum class Stage
{
    stageAvangard,
    stageFlank,
    stageRear
};

class TilesManager
{
public:
    TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side);

    void setStatus(const TilesManagerStatus &_status);
    void setUnitBuffer(Unit &_unit);
    Unit *getUnitBuffer();
    void setTileBuffer(Tile *_tile)
    {
        tileBuffer = _tile;
    };
    Tile *getTileBuffer()
    {
        return tileBuffer;
    };
    void mouseIsPressed();
    void updateFocus();
    void draw();
    void setTexture(Texture *_texture);
    bool hasEmptyTiles();
    void setStage(Stage _stage);
    void setButtons(Button *_btn1, Button *_btn2)
    {
        for (auto tile : tiles)
        {
            tile->setButtons(_btn1, _btn2);
        }
    }
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
    unique_ptr<Unit> unitBuffer;

    //Какой тайл будет атаковать/использовать способность
    Tile *tileBuffer;

    //Статус
    TilesManagerStatus status;
    //Стадия игры
    Stage stage;

    //Цвета для всех случаев
    const Color colorBasic = Color::White;
    const Color colorInFocus = Color(65, 105, 225);
    const Color colorForReleasingUnit = Color::Yellow;
    const Color colorWhenCantReleaseUnit = Color::Black;
    const Color colorForDeadBody = Color(100, 30, 100);
    const Color colorToBeAttacked = Color::Red;
    const Color colorWhenCantAttack = Color(30, 30, 30);
    const Color colorWhenCanAttack = Color(255, 162, 50);
    const Color colorWaitingForAttack = Color(200, 40, 240);
};