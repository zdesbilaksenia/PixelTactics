#include "GameManager.h"
#include "configurations.cpp"

GameManager::GameManager(RenderWindow &_window,
                         Mouse &_mouse,
                         Event &_event,
                         GameTcpClient *client,
                         ButtonsManager &_buttonsM,
                         TilesManager &_playerTM,
                         TilesManager &_opponentTM,
                         CardsManager &_cardsM,
                         Background &_background) : window(_window),
                                                    mouse(_mouse),
                                                    event(_event),
                                                    client(client),
                                                    buttonsManager(_buttonsM),
                                                    playerTilesManager(_playerTM),
                                                    opponentTilesManager(_opponentTM),
                                                    cardsManager(_cardsM),
                                                    background(_background),
                                                    stage(GameStage::stageStart),
                                                    round(RoundType::roundAvangard) {}

void GameManager::play()
{
    stage = GameStage::stageStart;
    round = RoundType::roundNoType;
    gameStart();
    //stage = GameStage::stageOpponentsTurn;
    stage = GameStage::stagePlayersTurn;

    opponentsTurn();

    //Ждём здесь, пока ответ о конце хода противника не придёт:

    round = RoundType::roundAvangard;
    while (window.isOpen() && stage != GameStage::stageGameOver)
    {
        switch (stage)
        {
        case (GameStage::stageOpponentsTurn):
            opponentsTurn();
            break;
        case (GameStage::stagePlayersTurn):
        {
            playRound();
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

void GameManager::gameStart()
{

    opponentTilesManager.disable();
    buttonsManager.disable();
    opponentTilesManager.updateFocus();
    buttonsManager.updateFocus();

    stage = GameStage::stageStart;
    playerTilesManager.setStatus(TilesManagerStatus::statusGameStarting);
    cardsManager.setStatus(CardsManagerStatus::statusGameStarting);

    for (int i = 0; i < 3; ++i)
    {
        cardsManager.takeCard();
    }

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
                //Если мы реализовали карту, то круто
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

        window.clear();

        background.draw();
        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();

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

void GameManager::opponentsTurn()
{
    cout << "GameManager::opponentsTurn(): Started!" << endl;

    playerTilesManager.disable();
    buttonsManager.disable();

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

        window.clear();

        background.draw();
        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();
    }
}

void GameManager::playersTurn()
{
    cout << "GameManager::playersTurn(): Started!" << endl;

    playerTilesManager.enable();
    playerTilesManager.updateFocus();

    opponentTilesManager.enable();
    opponentTilesManager.updateFocus();

    buttonsManager.enable();
    buttonsManager.updateFocus();

    int movesCount = 0;

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
                opponentTilesManager.updateFocus();
                cardsManager.updateFocus();
                buttonsManager.updateFocus();
                break;
            }
            case (Event::MouseButtonPressed):
            {
                //Порядок важен
                buttonsManager.mouseIsPressed();
                if (btnTakeCard->hasFocus())
                {
                    cout << "GameManager::playersTurn() : Card was taken!!" << endl;
                    movesCount++;
                    cout << "Moves count = " << movesCount << endl;
                }
                if (playerTilesManager.mouseIsPressed())
                {
                    cout << "GameManager::playersTurn() : Card was released!!" << endl;
                    movesCount++;
                    cout << "Moves count = " << movesCount << endl;
                }
                opponentTilesManager.mouseIsPressed();
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

        window.clear();

        background.draw();
        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();

        if (movesCount == 2)
        {
            cout << "GameManager::playersTurn() : Ended!!!" << endl;
            stage = GameStage::stageOpponentsTurn;
            return;
        }
    }
}

void GameManager::playRound()
{
    cout << "GameManager::playRound(): Started!" << endl;

    playerTilesManager.setRound(round);
    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);

    playerTilesManager.enable();
    playerTilesManager.updateFocus();

    opponentTilesManager.enable();
    opponentTilesManager.updateFocus();

    buttonsManager.enable();
    buttonsManager.updateFocus();

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
                    cout << "GameManager::playRound() : Got pressed!" << endl;
                }
                else
                {
                    btnRemoveBody->enable();
                    btnTakeCard->enable();
                    cout << "GameManager::playRound() : Did not got pressed!" << endl;
                }
                //Если атаковали
                if (opponentTilesManager.mouseIsPressed())
                {
                    cout << "GameManager::playRound() : Opponent's card was attacked!!" << endl;
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

        window.clear();

        background.draw();
        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();

        if (movesCount == 2)
        {
            cout << "GameManager::roundTurn() : Ended!!!" << endl;
            stage = GameStage::stageOpponentsTurn;
            return;
        }
    }
}