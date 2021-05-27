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
#include <thread>
#include <mutex>

auto loadPlayerHeroesStats{
    [](vector<Unit> &_units, vector<Breed> &_breeds, TilesManager &_tilesManager, const bool &_side) {
        for (int i = 0 + 9 * _side; i < 9 + 9 * _side; ++i)
        {
            int x = (i - 9 * (1 - _side)) / 3;
            int y = i % 3;
            if (_breeds[i].cardID != -1)
            {
                _units[_breeds[i].cardID].setTextAttack(_breeds[i].strength);
                _units[_breeds[i].cardID].setTextHP(_breeds[i].HP);
                cout << "unit ID = " << _breeds[i].cardID << " ___ ";
                _tilesManager.setUnit(_units[_breeds[i].cardID], x, y);
            }
        }
        return;
    }};

auto loadOpponentHeroesStats{
    [](vector<Unit> &_units, vector<Breed> &_breeds, TilesManager &_tilesManager, const bool &_side) {
        for (int i = 0 + 9 * (1 - _side); i < 9 + 9 * (1 - _side); ++i)
        {
            int x = (i - 9 * (1 - _side)) / 3;
            int y = i % 3;
            if (_breeds[i].cardID != -1)
            {
                _units[_breeds[i].cardID].setTextAttack(_breeds[i].strength);
                _units[_breeds[i].cardID].setTextHP(_breeds[i].HP);
                cout << "unit ID = " << _breeds[i].cardID << " ___ ";
                _tilesManager.setUnit(_units[_breeds[i].cardID], x, y);
            }
        }
        return;
    }};

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
                vector<Unit> &_opponentUnits,
                const vector<Texture> &_textures);

    void setAttackButton(Button &_btnAttack);
    void setPowerButton(Button &_btnPower);
    void setCancelButton(Button &_btnCancel);
    void setTakeCardButton(Button &_btnTakeCard);
    void setRemoveBodyButton(Button &_btnRemoveBody);

    void play();

    ~GameManager();

private:
    void loadBreeds(Message<GameMsgTypes> &msg)
    {
        vector<Breed> breeds;
        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Trying to load breed!";
        msg >> breeds;
        BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : breed loaded!";

        loadPlayerHeroesStats(playerUnits, breeds, playerTilesManager, side);
        loadOpponentHeroesStats(opponentUnits, breeds, opponentTilesManager, side);
    }

    //Текстуры для отрисовки противника
    const vector<Texture> &unitTextures;
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

    int side;
    //Вывел циклы для удобства

    void _whileForPlay()
    {
        while (window.isOpen() && stage != GameStage::stageWon && stage != GameStage::stageLost)
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
                if (stage == GameStage::stageWon)
                {
                    return;
                }
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
                        BOOST_LOG_TRIVIAL(info) << "GameManager::gameStart(): first card was successufully released!";
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

            if (firstCardIsReleased)
            {
                playerTilesManager.setStatus(TilesManagerStatus::statusCardWasJustReleased);
                cardsManager.setStatus(CardsManagerStatus::statusNothingHappens);
                opponentTilesManager.disable();
                buttonsManager.disable();
                opponentTilesManager.updateFocus();
                buttonsManager.updateFocus();

                if (client.incoming().empty())
                {
                    this->draw();
                }
                else
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : Got message from opponent!";

                    auto msg = client.incoming().popFront().msg;
                    vector<Breed> breeds;
                    if (msg.header.id == GameMsgTypes::GameHeroesStats)
                    {
                        BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : Trying to load breed!";
                        msg >> breeds;
                        BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : breed loaded!";
                    }

                    loadPlayerHeroesStats(playerUnits, breeds, playerTilesManager, side);
                    loadOpponentHeroesStats(opponentUnits, breeds, opponentTilesManager, side);

                    opponentTilesManager.updateFocus();
                    playerTilesManager.updateFocus();
                    buttonsManager.updateFocus();

                    return;
                }
            }
            else
            {
                this->draw();
            }
        }
    }

    void _whileForOpponentsTurn()
    {
        int moveAmounts = 0;

        while (window.isOpen())
        {
            while (window.pollEvent(event))
            {
                switch (event.type)
                {
                case (Event::MouseMoved):
                {
                    opponentTilesManager.updateFocus();
                    cardsManager.updateFocus();
                    break;
                }
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

            if (client.incoming().empty())
            {
                this->draw();
            }
            else
            {
                moveAmounts++;

                cout << "Here" << endl;
                BOOST_LOG_TRIVIAL(info) << client.getSide() << " GameManager::opponentsTurn() : Got message from opponent!";

                auto msg = client.incoming().popFront().msg;
                switch (msg.header.id)
                {
                case GameMsgTypes::GameHeroesStats:
                {
                    vector<Breed> breeds;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : Trying to load breed!";
                    msg >> breeds;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : breed loaded!";

                    loadPlayerHeroesStats(playerUnits, breeds, playerTilesManager, side);
                    loadOpponentHeroesStats(opponentUnits, breeds, opponentTilesManager, side);

                    playerTilesManager.updateFocus();
                    opponentTilesManager.updateFocus();
                    buttonsManager.updateFocus();

                    break;
                }
                //Если противник взял карту
                case GameMsgTypes::GameTakeCard:
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : Opponent took card!";
                    break;
                }
                case GameMsgTypes::GameCardReleased:
                {
                    int posY;
                    msg >> posY;
                    int posX;
                    msg >> posX;
                    int attack;
                    msg >> attack;
                    int HP;
                    msg >> HP;
                    int ID;
                    msg >> ID;
                    cout << "ID = " << ID << " posX = " << posX << " posY = " << posY << endl;
                    opponentUnits[ID].setTextAttack(attack);
                    opponentUnits[ID].setTextHP(HP);
                    opponentTilesManager.setUnit(opponentUnits[ID], posX, posY);
                    opponentTilesManager.updateFocus();
                    break;
                }
                case GameMsgTypes::GameLost:
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : You lost!";
                    stage = GameStage::stageLost;
                    return;
                    break;
                }
                default:
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : Some other type";
                    break;
                }
                }

                if (moveAmounts == 2)
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn(): Ended!";
                    stage = GameStage::stagePlayersTurn;
                    return;
                }
            }
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

                        //Мб не while
                        while (client.incoming().empty())
                        {
                            this->draw();
                        }
                        auto msg = client.incoming().popFront().msg;
                        switch (msg.header.id)
                        {
                        case GameMsgTypes::GameHeroesStats:
                        {
                            vector<Breed> breeds;
                            BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Trying to load breed!";
                            msg >> breeds;
                            BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : breed loaded!";

                            loadPlayerHeroesStats(playerUnits, breeds, playerTilesManager, side);
                            loadOpponentHeroesStats(opponentUnits, breeds, opponentTilesManager, side);

                            break;
                        }
                        default:
                        {
                            BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Some other type";
                            break;
                        }
                        }

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

            //////////////////////////
            this->draw();
            /////////////////////////

            if (movesCount == 2)
            {
                BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn(): Ended!";
                stage = GameStage::stageOpponentsTurn;
                return;
            }
        }
    }
};
