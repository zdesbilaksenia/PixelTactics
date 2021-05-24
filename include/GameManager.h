#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
using namespace std;

#include <boost/log/trivial.hpp>
using namespace boost::log;

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
                GameTcpClient &client,
                ButtonsManager &_buttonsM,
                TilesManager &_playerTM,
                TilesManager &_opponentTM,
                CardsManager &_cardsM,
                Background &_background,
                const vector<Texture>& _textures);

    void setAttackButton(Button &_btnAttack);
    void setPowerButton(Button &_btnPower);
    void setCancelButton(Button &_btnCancel);
    void setTakeCardButton(Button &_btnTakeCard);
    void setRemoveBodyButton(Button &_btnRemoveBody);

    void play();

private:

    //Текстуры для отрисовки противника
    const vector<Texture>& unitTextures;
    //Юниты противника
    vector<Unit> opponentUnits;

    RenderWindow &window;
    Mouse &mouse;
    Event &event;
    GameTcpClient &client;
    ButtonsManager &buttonsManager;
    TilesManager &playerTilesManager;
    TilesManager &opponentTilesManager;
    CardsManager &cardsManager;
    Background &background;
    //Нужно сделать класс "Label", который будет рисовать текст

    unique_ptr<Button> btnAttack;
    unique_ptr<Button> btnPower;
    unique_ptr<Button> btnCancel;
    unique_ptr<Button> btnTakeCard;
    unique_ptr<Button> btnRemoveBody;

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

    void draw();
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
                default:
                    break;
                }
                stage = GameStage::stageOpponentsTurn;
                break;
            }
            default:
                break;
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
                        BOOST_LOG_TRIVIAL(info) << "GameManager::gameStart(): Card was successufully released!";
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
                        BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn(): Ended!";
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
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Card was taken!";
                        movesCount++;
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesCount;
                    }
                    //Если реализовали карту
                    if (playerTilesManager.mouseIsPressed())
                    {
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Card was released!";
                        movesCount++;
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesCount;
                        //Это должно быть в этой строчке, поскольку функция отлавливает факт того, что карта была выложена на стол.
                        cardsManager.mouseIsPressed();
                        //Чтобы отрисовывались только те тайлы, которыми мы можем управлять.
                        playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                        playerTilesManager.updateFocus();
                    }
                    //Если нажали на тайл (Чтобы атаковать)
                    if (playerTilesManager.getPressed())
                    {
                        //Здесь баг, кнопка активируется, если нажмём в другом месте
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
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Opponent's unit was attacked!";
                        movesCount++;
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesCount;
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
                BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn(): Ended!";
                stage = GameStage::stageOpponentsTurn;
                return;
            }
        }
    }

};
