#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Button.h"
#include "Card.h"
#include "Tile.h"
#include "Background.h"

class GameManager
{
public:
    GameManager(RenderWindow &_window,
                Mouse &_mouse,
                Event &_event,
                GameTcpClient *client,
                ButtonsManager &_buttonsM,
                TilesManager &_playerTM,
                TilesManager &_opponentTM,
                CardsManager &_cardsM,
                Background &_background);

    void setAttackButton(Button *_btnAttack)
    {
        btnAttack = _btnAttack;
    }

    void setPowerButton(Button *_btnPower)
    {
        btnPower = _btnPower;
    }

    void setCancelButton(Button *_btnCancel)
    {
        btnCancel = _btnCancel;
    }

    void setTakeCardButton(Button *_btnTakeCard)
    {
        btnTakeCard = _btnTakeCard;
    }

    void setRemoveBodyButton(Button *_btnRemoveBody)
    {
        btnRemoveBody = _btnRemoveBody;
    }

    void play();

private:
    RenderWindow &window;
    Mouse &mouse;
    Event &event;
    GameTcpClient *client;
    ButtonsManager &buttonsManager;
    TilesManager &playerTilesManager;
    TilesManager &opponentTilesManager;
    CardsManager &cardsManager;
    Background &background;
    //Нужно сделать класс "Label", который будет рисовать текст

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
        stageReleasingCard
    };

    GameStage stage;
    RoundType round;

    void gameStart();
    void playersTurn();
    void opponentsTurn();
    void playRound();
};
