#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
using std::cout;
using std::endl;

#include <boost/log/trivial.hpp>
using namespace boost::log;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Button.h"
#include "Card.h"
#include "Tile.h"
#include "Background.h"
#include <thread>
#include <mutex>

class GameManager
{
public:
    GameManager(RenderWindow &_window,
                Mouse &_mouse,
                Event &_event,
                GameTcpClient &client,
                ButtonsManager &_buttonsM,
                TilesManager &_playerTM,
                TilesManager &_opponentTM,
                CardsManager &_cardsM,
                Background &_background,
                vector<Unit> &_platerUnits,
                vector<Unit> &_opponentUnits);

    void setAttackButton(Button &_btnAttack);
    void setPowerButton(Button &_btnPower);
    void setCancelButton(Button &_btnCancel);
    void setTakeCardButton(Button &_btnTakeCard);
    void setRemoveBodyButton(Button &_btnRemoveBody);

    void play();

    ~GameManager();

private:
    void loadBreeds(Message<GameMsgTypes> &msg);
    //Юниты противника
    vector<Unit> &playerUnits;
    vector<Unit> &opponentUnits;

    RenderWindow &window;
    Mouse &mouse;
    Event &event;
    GameTcpClient &client;
    ButtonsManager &buttonsManager;
    TilesManager &playerTilesManager;
    TilesManager &opponentTilesManager;
    CardsManager &cardsManager;
    Background &background;

    Button *btnAttack;
    Button *btnPower;
    Button *btnCancel;
    Button *btnTakeCard;
    Button *btnRemoveBody;

    enum class GameStage
    {
        stageStart,
        stagePlayersTurn,
        stageOpponentsTurn,
        stageAttack,
        stagePower,
        stageGameOver,
        stageReleasingCard,
        stageLost,
        stageWon
    };

    GameStage stage;
    RoundType round;

    void draw();
    void gameStart();
    void playersTurn();
    void opponentsTurn();

    bool handleMessage(const GameMsgTypes &msgType);
    bool handleMessageFromOpponent();

    int side;

    void _whileForPlay();
    void _whileForGameStart();
    void _whileForOpponentsTurn();
    void _whileForPlayersTurn();
};
