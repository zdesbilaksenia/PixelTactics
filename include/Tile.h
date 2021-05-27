#pragma once

//#include "DrawableBox.h"
#include "Clickable.h"
#include "Unit.h"
#include "Button.h"
#include <vector>
#include <memory>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
using std::make_unique;
using std::unique_ptr;
using std::vector;
#include <boost/log/trivial.hpp>
using namespace boost::log;

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
    Unit *getUnit();
    int &getCoordX();
    int &getCoordY();

    //coordX - столбец
    //coordY - строка
    void setCoordinates(const int &_x, const int &_y);
    TileStatus getStatus();
    ~Tile();

private:
    //Button *buttonAttack;
    //Button *buttonPower;

    int coordX;
    int coordY;

    Unit *unit;
    //Unit *unit;
    TileStatus status;
    const Color colorForAttack = Color(255, 111, 0);

    //Метод для
    bool sendAttackMessage(int coordX1, int coordY1, int coordX2, int coordY2);
};

//Например, игрок нажал на кнопку "Аттаковать"
//Сама комманда не знает, кого он аттакует, и она не может "ждать"
//Сохраняем статус комманды в enum и уже в цикле обработки сообщений будем от него отталкиваться.
//
enum class TilesManagerStatus
{
    //Когда ничего не происходит
    statusNothingHappens,

    //Начинаем игру
    statusGameStarting,
    //Реализуем карту в начале игры
    statusGameStartingReleasingCard,

    //Готовы выложить карту
    statusReleasingCard,
    //Готовы атаковать юнита противника
    statusAttackingUnit,
    //Только что реализовали карту, статус нужен для CardManager
    statusCardWasJustReleased,
    //Когда атакуем тайлы противника
    statusWaitingForAttack,
    //Когда используем силу на тайлы противника
    statusWaitingForPower,
    //Когда была использована способность, которая сразу меняет статы
    statusWhenThePowerWhichChangesStatsImmidiatelyWasActivated,
    //Когда ждём, что уберут тело
    statusWaitingForRemovingBody,
    //Когда тело убрали
    statusBodyRemoved
};

enum class Side
{
    sidePlayer,
    sideOpponent
};

enum class RoundType
{
    roundNoType,
    roundAvangard,
    roundFlank,
    roundRear
};

class TilesManager
{
public:
    TilesManager(RenderWindow &_window, Mouse &_mouse, const Side &_side, GameTcpClient &_client);

    void setStatus(const TilesManagerStatus &_status);
    void setUnitBuffer(Unit &_unit);
    void setTileBuffer(Tile *_tile);
    Tile *getTileBuffer();
    bool mouseIsPressed();
    void updateFocus();
    void draw();
    void setTexture(Texture *_texture);
    bool hasEmptyTiles();
    void setRound(RoundType &_round);
    TilesManagerStatus getStatus();
    void enable();
    void disable();
    bool getPressed();
    void setActiveTiles(vector<bool> &_activeTiles);
    void resetActiveTiles();

    //В том случае, если TilesManager - для отрисовки тайлов оппонента
    void setUnit(Unit &unit, RoundType round, int coordY)
    {
        switch (round)
        {
        case (RoundType::roundAvangard):
        {
            tilesAvangard[coordY]->setUnit(unit);
            break;
        }
        case (RoundType::roundFlank):
        {
            tilesFlank[coordY]->setUnit(unit);
            break;
        }
        case (RoundType::roundRear):
        {
            tilesRear[coordY]->setUnit(unit);
            break;
        }
        default:
            break;
        }
    }

    void setUnit(Unit &unit, int coordX, int coordY)
    {
        switch (coordX)
        {
        case 0:
            setUnit(unit, RoundType::roundAvangard, coordY);
            break;
        case 1:
            setUnit(unit, RoundType::roundFlank, coordY);
            break;
        case 2:
            setUnit(unit, RoundType::roundRear, coordY);
            break;
        default:
            break;
        }
    }

    ~TilesManager();

private:
    GameTcpClient &client;
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
    Unit *unitBuffer;

    //Какой тайл будет атаковать/использовать способность
    Tile *tileBuffer;

    //Статус
    TilesManagerStatus status;
    //Тип раунда
    RoundType round;

    bool isPressed;

    //Цвета для всех случаев
    const Color colorBasic = Color::White;
    const Color colorDisabled = Color(150, 150, 150);
    const Color colorInFocus = Color(65, 105, 225);
    const Color colorForReleasingUnit = Color::Yellow;
    const Color colorWhenCantReleaseUnit = Color::Black;
    const Color colorForDeadBody = Color(100, 30, 100);
    const Color colorToBeAttacked = Color::Red;
    const Color colorWhenCantAttack = Color(30, 30, 30);
    const Color colorWhenCanAttack = Color(255, 162, 50);
    const Color colorWaitingForAttack = Color(200, 40, 240);

    bool activeTiles[3][3];
};