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
                vector<Unit> &_opponentUnits,
                const vector<Texture> &_textures);

    void setAttackButton(Button &_btnAttack);
    void setPowerButton(Button &_btnPower);
    void setCancelButton(Button &_btnCancel);
    void setTakeCardButton(Button &_btnTakeCard);
    void setRemoveBodyButton(Button &_btnRemoveBody);

    void play();

private:
    //Текстуры для отрисовки противника
    const vector<Texture> &unitTextures;
    //Юниты противника
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

    int side;
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
                        //В начале игры мы можем только выложить карту на стол.
                        BOOST_LOG_TRIVIAL(info) << "GameManager::gameStart(): Card was successufully released!";
                        firstCardIsReleased = true;
                        //Эту информацию сразу отлавливает CardsManager
                        playerTilesManager.setStatus(TilesManagerStatus::statusCardWasJustReleased);
                    }
                    if (cardsManager.mouseIsPressed())
                    {
                        playerTilesManager.setStatus(TilesManagerStatus::statusGameStartingReleasingCard);
                        cardsManager.setStatus(CardsManagerStatus::statusGameStartingReleasingCard);
                        playerTilesManager.updateFocus();
                    }
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

//#if SERVER_CONNECTING == 1

                BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : Waiting for opponent!";
                client.incoming().wait();
                auto msg = client.incoming().popFront().msg;
                vector<Breed> breed;
                if (msg.header.id == GameMsgTypes::GameHeroesStats)
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : Trying to load breed!";
                    msg >> breed;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : breed loaded!";
                }
                cout << "======================" << endl;

                //1-му игроку - от 9 до 18, второму - наоборот
                int id = 0;
                for (int i = 0 + 9 * (1 - side); i < 9 + 9 * (1 - side); ++i)
                {
                    if (breed[i].cardID != -1)
                    {
                        id = breed[i].cardID;
                    }
                    cout << "cardID = " << breed[i].cardID << " strength = " << breed[i].strength << " HP = " << breed[i].HP << endl;
                }

                opponentTilesManager.setUnit(opponentUnits[id], RoundType::roundFlank, 1);
                opponentTilesManager.draw();
//#endif

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
                    opponentTilesManager.updateFocus();
                    cardsManager.updateFocus();
                    break;
                }

                //ИСПРАВИТЬ!!!
                //Необходимо ждать ответ от сервера
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
                        //Перенёс строчку из TilesManager::mouseIsPressed() case TilesManagerStatus::statusWaitingForAttack
                        playerTilesManager.setStatus(TilesManagerStatus::statusCardWasJustReleased);
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
                    //Если атаковали противника
                    if (opponentTilesManager.mouseIsPressed())
                    {
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Opponent's tile was attacked succesfully!";
                        movesCount++;
                        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesCount;
                        opponentTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                        opponentTilesManager.updateFocus();
                        playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                        playerTilesManager.updateFocus();
                    }
                    if (cardsManager.mouseIsPressed())
                    {
                        playerTilesManager.setStatus(TilesManagerStatus::statusReleasingCard);
                        cardsManager.setStatus(CardsManagerStatus::statusReleasingCard);
                    }
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
