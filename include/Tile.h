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
{
public:
    Tile(RenderWindow &_window, Mouse &_mouse);
    void setUnit(Unit &_unit);
    void setStatus(TileStatus _status);
    void draw() override;
    Unit *getUnit();
    int &getCoordX();
    int &getCoordY();
    void deleteUnit();
    void updateUnitFocus();

    //coordX - столбец
    //coordY - строка
    void setCoordinates(const int &_x, const int &_y);
    TileStatus getStatus();
    ~Tile();

private:
    int coordX;
    int coordY;

    Unit *unit;
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
    //Когда недоступен
    statusDisabled,
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
    statusBodyRemoved,
    //Когда была использована магия
    statusPowerWasUsed
};

enum class Side
{
    sidePlayer,
    sideOpponent
};

enum class RoundType
{
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
    void updateFocus();
    void draw();
    void setTexture(Texture *_texture);
    bool hasEmptyTiles();
    void setRound(RoundType &_round);
    TilesManagerStatus getStatus();
    void enable();
    void disable();
    bool getPressed();
    void setActiveTiles(const vector<bool> &_activeTiles);
    void resetActiveTiles();
    bool handleClick(Tile &tile);
    bool mouseClicked();
    bool removedBody();
    bool powerWasUsed();
    void deleteUnit(int x, int y);
    void setUnit(Unit &unit, int coordX, int coordY);
    bool hasBodies();

    void setActiveRoundTiles();

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

    Color colorActive;
    Color colorPassive;

    bool activeTiles[3][3];
};

    //Цвета для всех случаев
    const Color colorBasic = Color::White;
    const Color colorDisabled = Color(150, 150, 150);
    const Color colorInFocus = Color(65, 105, 225);
    const Color colorForReleasingUnit = Color::Yellow;
    const Color colorWhenCantReleaseUnit = Color::Black;
    const Color colorForDeadBody = Color(100, 12, 171);
    const Color colorToBeAttacked = Color(247, 156, 255);
    const Color colorWhenCantAttack = Color(30, 30, 30);
    const Color colorWhenCanAttack = Color(255, 162, 50);
    const Color colorWaitingForAttack = Color(200, 40, 240);
    const Color colorForRemovingBody = Color::Blue;
    const Color colorForPower = Color(74, 201, 255);