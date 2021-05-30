#include "GameManager.h"
#include "configurations.cpp"

auto loadHeroesStats{
    [](vector<Unit> &_units, vector<Breed> &_breeds, TilesManager &_tilesManager, const bool &_side)
    {
        for (int i = 0 + 9 * _side; i < 9 + 9 * _side; ++i)
        {
            int x = (i - 9 * _side) / 3;
            int y = i % 3;
            if (_breeds[i].cardID != -1)
            {
                _units[_breeds[i].cardID].setTextAttack(_breeds[i].strength);
                _units[_breeds[i].cardID].setTextHP(_breeds[i].HP);
                _tilesManager.setUnit(_units[_breeds[i].cardID], x, y);
            }
            else
            {
                _tilesManager.deleteUnit(x, y);
            }
        }
        return;
    }};

GameManager::GameManager(RenderWindow &_window,
                         Mouse &_mouse,
                         Event &_event,
                         GameTcpClient &client,
                         ButtonsManager &_buttonsM,
                         TilesManager &_playerTM,
                         TilesManager &_opponentTM,
                         CardsManager &_cardsM,
                         Background &_background,
                         vector<Unit> &_playerUnits,
                         vector<Unit> &_opponentUnits) : window(_window),
                                                         mouse(_mouse),
                                                         event(_event),
                                                         client(client),
                                                         buttonsManager(_buttonsM),
                                                         playerTilesManager(_playerTM),
                                                         opponentTilesManager(_opponentTM),
                                                         cardsManager(_cardsM),
                                                         background(_background),
                                                         stage(GameStage::stageStart),
                                                         round(RoundType::roundAvangard),
                                                         playerUnits(_playerUnits),
                                                         opponentUnits(_opponentUnits)
{
    side = client.getSide();
    BOOST_LOG_TRIVIAL(info) << "GameManager::GameManager() : side = " << side;
}

void GameManager::setAttackButton(Button &_btnAttack)
{
    btnAttack = &_btnAttack;
}

void GameManager::setPowerButton(Button &_btnPower)
{
    btnPower = &_btnPower;
}

void GameManager::setCancelButton(Button &_btnCancel)
{
    btnCancel = &_btnCancel;
}

void GameManager::setTakeCardButton(Button &_btnTakeCard)
{
    btnTakeCard = &_btnTakeCard;
}

void GameManager::setRemoveBodyButton(Button &_btnRemoveBody)
{
    btnRemoveBody = &_btnRemoveBody;
}

void GameManager::draw()
{
    window.clear();

    background.draw();
    buttonsManager.draw();
    playerTilesManager.draw();
    opponentTilesManager.draw();
    cardsManager.draw();

    window.display();
}

void GameManager::play()
{
    stage = GameStage::stageStart;
    round = RoundType::roundAvangard;
    gameStart();
    stage = GameStage::stagePlayersTurn;

    //Если второй игрокvoid
    if (side == 1)
    {
        //Ждём здесь, пока ответ о конце хода противника не придёт:
        opponentsTurn();
    }

    round = RoundType::roundAvangard;

    _whileForPlay();

    if (stage == GameStage::stageLost)
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::play(): You lost!";
        return;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::play(): You won!";
        return;
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

    vector<bool> activeTiles =
        {1, 1, 1,
         1, 1, 1,
         1, 1, 1};
    playerTilesManager.setActiveTiles(activeTiles);

    //Вначале игры берутся 3 карты
    for (int i = 0; i < 3; ++i)
    {
        cardsManager.takeCard();
    }

    this->draw();

    _whileForGameStart();
}

void GameManager::opponentsTurn()
{
    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn(): Started!";

    playerTilesManager.disable();
    buttonsManager.disable();

    _whileForOpponentsTurn();

    //Won - в том случае, если противник ливнёт
    if (stage == GameStage::stageLost || stage == GameStage::stageWon)
    {
        return;
    }
}

void GameManager::playersTurn()
{
    playerTilesManager.setTileBuffer(nullptr);
    opponentTilesManager.setTileBuffer(nullptr);

    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn(): Started!";

    playerTilesManager.setRound(round);
    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);

    playerTilesManager.enable();
    opponentTilesManager.enable();
    buttonsManager.enable();

    //Возможно, неправильно работает, не проверял
    playerTilesManager.setActiveRoundTiles();

    _whileForPlayersTurn();

    if (stage == GameStage::stageWon)
    {
        return;
    }
}

bool GameManager::handleMessage(const GameMsgTypes &msgType)
{
    client.incoming().wait();
    auto msg = client.incoming().popFront().msg;
    if (msg.header.id == msgType)
    {
        switch (msgType)
        {
        case GameMsgTypes::GameHeroesStats:
        {
            loadBreeds(msg);
            return true;
            break;
        }
        default:
        {
            return false;
            break;
        }
            //Доделать, возможно, это и не нужно.
        }
    }
    else if (msg.header.id == GameMsgTypes::GameWon)
    {
        stage = GameStage::stageWon;
        return false;
    }
    else if (msg.header.id == GameMsgTypes::GameLost)
    {
        stage = GameStage::stageLost;
        return false;
    }
    return false;
}

void GameManager::handleMessageFromOpponent()
{
    auto msg = client.incoming().popFront().msg;
    switch (msg.header.id)
    {
    case GameMsgTypes::GameHeroesStats:
    {
        loadBreeds(msg);
        break;
    }
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
    case GameMsgTypes::GameWon:
    {
        BOOST_LOG_TRIVIAL(info) << "1 GameManager::opponentsTurn() : You Won!";
        stage = GameStage::stageWon;
        return;
        break;
    }
    default:
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : Some other type";
        break;
    }
    }
}

void GameManager::_whileForPlay()
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
            if (stage == GameStage::stageWon)
            {
                return;
            }
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
        default:
            break;
        }
    }
}

void GameManager::_whileForGameStart()
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
                playerTilesManager.mouseClicked();
                if (!firstCardIsReleased)
                {
                    cardsManager.mouseClicked();
                }
                if (cardsManager.numberOfCardsInHand() == 2)
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::gameStart(): first card was successufully released!";
                    firstCardIsReleased = true;
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
            playerTilesManager.disable();
            opponentTilesManager.disable();
            buttonsManager.disable();

            if (client.incoming().empty())
            {
                this->draw();
            }
            else
            {
                BOOST_LOG_TRIVIAL(info) << "GameManager::ForGamesStart() : Got message from opponent!";
                auto msg = client.incoming().popFront().msg;
                if (msg.header.id == GameMsgTypes::GameHeroesStats)
                {
                    loadBreeds(msg);
                }
                return;
            }
        }
        else
        {
            this->draw();
        }
    }
}

void GameManager::_whileForOpponentsTurn()
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
                playerTilesManager.updateFocus();
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
            BOOST_LOG_TRIVIAL(info) << client.getSide() << " GameManager::opponentsTurn() : Got message from opponent!";
            //Возможно, здесь может быть ошибка, необходимо проверить
            handleMessageFromOpponent();
            if (moveAmounts == 2)
            {
                BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn(): Ended!";
                stage = GameStage::stagePlayersTurn;
                return;
            }
        }
    }
}

void GameManager::_whileForPlayersTurn()
{
    int movesAmount = 0;
    if (!playerTilesManager.hasBodies())
    {
        btnRemoveBody->disable();
    }
    if (!cardsManager.canTakeCard())
    {
        btnTakeCard->disable();
    }

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
                //Надо сделать if (cardsManager.getCardWasTaken()) и проверить, нормально ли работает
                if (cardsManager.getCardWasTaken() && btnTakeCard->hasFocus())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Card was taken!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                }

                if (playerTilesManager.mouseClicked())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Move was done!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.updateFocus();
                }

                if (playerTilesManager.powerWasUsed())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Power was used on player's tile!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    if (!handleMessage(GameMsgTypes::GameHeroesStats))
                    {
                        return;
                    }
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }

                if (opponentTilesManager.powerWasUsed())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Power was used on opponent's tile!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    client.incoming().wait();
                    if (!handleMessage(GameMsgTypes::GameHeroesStats))
                    {
                        return;
                    }
                    opponentTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                    opponentTilesManager.updateFocus();
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }

                if (playerTilesManager.removedBody())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Body was removed!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    client.incoming().wait();
                    if (!handleMessage(GameMsgTypes::GameHeroesStats))
                    {
                        return;
                    }
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }

                //Если атаковали противника
                if (opponentTilesManager.mouseClicked())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Opponent's tile was attacked succesfully!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    while (client.incoming().empty())
                    {
                        this->draw();
                    }
                    if (!handleMessage(GameMsgTypes::GameHeroesStats))
                    {
                        return;
                    }
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    opponentTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                    opponentTilesManager.updateFocus();
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }

                cardsManager.mouseClicked();
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

        if (movesAmount == 2)
        {
            BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn(): Ended!";
            stage = GameStage::stageOpponentsTurn;
            playerTilesManager.resetActiveTiles();
            playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
            playerTilesManager.updateFocus();
            opponentTilesManager.resetActiveTiles();
            opponentTilesManager.updateFocus();
            return;
        }
    }
}

void GameManager::loadBreeds(Message<GameMsgTypes> &msg)
{
    vector<Breed> breeds;
    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Trying to load breed!";
    msg >> breeds;
    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : breed loaded!";

    loadHeroesStats(playerUnits, breeds, playerTilesManager, side);
    loadHeroesStats(opponentUnits, breeds, opponentTilesManager, (1 - side));

    playerTilesManager.updateFocus();
    opponentTilesManager.updateFocus();
    buttonsManager.updateFocus();
}

GameManager::~GameManager()
{
    btnAttack = nullptr;
    btnPower = nullptr;
    btnCancel = nullptr;
    btnTakeCard = nullptr;
    btnRemoveBody = nullptr;
}