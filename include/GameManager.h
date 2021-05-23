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

    void draw()
    {
        window.clear();

        background.draw();
        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();
    }

    void gameStart();
    void playersTurn();
    void opponentsTurn();

    //Вывел циклы для удобства

    void _whileForPlay()
    {
        while (window.isOpen() && stage != GameStage::stageGameOver)
        {
            switch (stage)
            {
            case (GameStage::stageOpponentsTurn):
                opponentsTurn();
                break;
            case (GameStage::stagePlayersTurn):
            {
                playersTurn();
                switch (round)
                {
                case (RoundType::roundAvangard):
                    round = RoundType::roundFlank;
                    break;
                case (RoundType::roundFlank):
                    round = RoundType::roundRear;
                    break;
                case (RoundType::roundRear):
                    round = RoundType::roundAvangard;
                    break;
                }
                stage = GameStage::stageOpponentsTurn;
                break;
            }
            }
        }
    }

    void _whileForGameStart()
    {
        bool firstCardIsReleased = false;
        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                //Если двигаем мышкой
                case (Event::MouseMoved):
                {
                    playerTilesManager.updateFocus();
                    cardsManager.updateFocus();
                    break;
                }
                //Если нажали на кнопку на мыши
                case (Event::MouseButtonPressed):
                {
                    if (playerTilesManager.mouseIsPressed())
                    {
                        cout << "GameManager::gameStart(): Card was successufully released!!!" << endl;
                        firstCardIsReleased = true;
                    }
                    cardsManager.mouseIsPressed();
                    break;
                }
                //Закрытие окна
                case (Event::Closed):
                {
                    window.close();
                }
                default:
                    break;
                }
            }

            this->draw();

            if (firstCardIsReleased)
            {
                playerTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                cardsManager.setStatus(CardsManagerStatus::statusNothingHappens);
                opponentTilesManager.enable();
                buttonsManager.enable();
                opponentTilesManager.updateFocus();
                buttonsManager.updateFocus();
                return;
            }
        }
    }

    void _whileForOpponentsTurn()
    {
        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                //Если двигаем мышкой
                case (Event::MouseMoved):
                {
                    cardsManager.updateFocus();
                    break;
                }

                //ИСПРАВИТЬ!!!
                //Необходимо ждать ответ от сервера
                //И у всей функции, наверное, стоит сделать bool
                case (Event::KeyPressed):
                {
                    if (Keyboard::isKeyPressed(Keyboard::A))
                    {
                        playerTilesManager.enable();
                        buttonsManager.enable();
                        playerTilesManager.updateFocus();
                        buttonsManager.updateFocus();
                        stage = GameStage::stagePlayersTurn;
                        cout << "GameManager::opponentsTurn(): Ended!" << endl;
                        return;
                    }
                    break;
                }
                //

                //Закрытие окна
                case (Event::Closed):
                {
                    window.close();
                    return;
                    break;
                }
                default:
                    break;
                }
            }

            this->draw();
        }
    }

    void _whileForPlayersTurn()
    {
        int movesCount = 0;

        while (window.isOpen())
        {
            //Если не можем взять карту.
            if (cardsManager.canTakeCard() == false)
            {
                btnTakeCard->disable();
            }

            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                //Если двигаем мышкой
                case (Event::MouseMoved):
                {
                    playerTilesManager.updateFocus();
                    opponentTilesManager.updateFocus();
                    cardsManager.updateFocus();
                    buttonsManager.updateFocus();
                    break;
                }
                case (Event::MouseButtonPressed):
                {
                    //Порядок важен
                    buttonsManager.mouseIsPressed();
                    if (btnTakeCard->isEnabled() && btnTakeCard->hasFocus())
                    {
                        cout << "GameManager::playRound() : Card was taken!!" << endl;
                        movesCount++;
                        cout << "GameManager::playRound() : Moves count = " << movesCount << endl;
                    }
                    //Если реализовали карту
                    if (playerTilesManager.mouseIsPressed())
                    {
                        cout << "GameManager::playRound() : Card was released!!" << endl;
                        movesCount++;
                        cout << "GameManager::playRound() : Moves count = " << movesCount << endl;
                        //Это должно быть в этой строчке, поскольку функция отлавливает факт того, что карта была выложена на стол.
                        cardsManager.mouseIsPressed();
                        //Чтобы отрисовывались только те тайлы, которыми мы можем управлять.
                        playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                        playerTilesManager.updateFocus();
                    }
                    //Если нажали на тайл (Чтобы атаковать)
                    if (playerTilesManager.getPressed())
                    {
                        btnRemoveBody->disable();
                        btnTakeCard->disable();
                    }
                    else
                    {
                        btnRemoveBody->enable();
                        btnTakeCard->enable();
                    }
                    //Если атаковали
                    if (opponentTilesManager.mouseIsPressed())
                    {
                        cout << "GameManager::playRound() : Opponent's unit was attacked!!" << endl;
                        movesCount++;
                        cout << "GameManager::playRound() : Moves count = " << movesCount << endl;
                        playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                        playerTilesManager.updateFocus();
                    }
                    cardsManager.mouseIsPressed();
                    break;
                }
                //Если отпустили кнопку на мыши
                case (Event::MouseButtonReleased):
                {
                    buttonsManager.mouseIsReleased();
                    break;
                }

                //Закрытие окна
                case (Event::Closed):
                {
                    window.close();
                    return;
                    break;
                }
                default:
                    break;
                }
            }

            this->draw();

            if (movesCount == 2)
            {
                cout << "GameManager::roundTurn() : Ended!!!" << endl;
                stage = GameStage::stageOpponentsTurn;
                return;
            }
        }
    }
};
