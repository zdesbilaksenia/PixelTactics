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
                         vector<Unit> &_opponentUnits,
                         StageText &_stageText) : window(_window),
                                                  mouse(_mouse),
                                                  event(_event),
                                                  client(client),
                                                  buttonsManager(_buttonsM),
                                                  playerTilesManager(_playerTM),
                                                  opponentTilesManager(_opponentTM),
                                                  cardsManager(_cardsM),
                                                  background(_background),
                                                  playerUnits(_playerUnits),
                                                  opponentUnits(_opponentUnits),
                                                  stageText(_stageText),
                                                  stage(GameStage::stageStart),
                                                  round(RoundType::roundAvangard)
{
    side = client.getSide();
    BOOST_LOG_TRIVIAL(info) << "GameManager::GameManager() : side = " << side;
}

void GameManager::setSpecialButtons(Button &_btnAttack, Button &_btnPower, Button &_btnTakeCard, Button &_btnRemoveBody)
{
    btnAttack = &_btnAttack;
    btnPower = &_btnPower;
    btnTakeCard = &_btnTakeCard;
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
    stageText.draw();

    window.display();
}

void GameManager::play()
{
    stage = GameStage::stageStart;
    round = RoundType::roundAvangard;
    gameStart();
    stage = GameStage::stagePlayersTurn;

    //Если второй игрок
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

    stageText.setRound(Round::start);
    stageText.restart();

    _whileForGameStart();
}

void GameManager::opponentsTurn()
{
    BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn(): Started!";

    stageText.setRound(Round::opponentsTurn);
    stageText.restart();

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

    playerTilesManager.setActiveRoundTiles();

    if (round == RoundType::roundAvangard)
    {
        stageText.setRound(Round::avangard);
    }
    else if (round == RoundType::roundFlank)
    {
        stageText.setRound(Round::flank);
    }
    else if (round == RoundType::roundRear)
    {
        stageText.setRound(Round::rear);
    }
    stageText.restart();

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

bool GameManager::handleMessageFromOpponent()
{
    auto msg = client.incoming().popFront().msg;
    switch (msg.header.id)
    {
    case GameMsgTypes::GameHeroesStats:
    {
        loadBreeds(msg);
        return true;
        break;
    }
    case GameMsgTypes::GameTakeCard:
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : Opponent took card!";
        return true;
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
        return true;
        break;
    }
    case GameMsgTypes::GameLost:
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : You lost!";
        stage = GameStage::stageLost;
        return false;
        break;
    }
    case GameMsgTypes::GameWon:
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::opponentsTurn() : You Won!";
        stage = GameStage::stageWon;
        return false;
        break;
    }
    default:
    {
        BOOST_LOG_TRIVIAL(error) << "GameManager::opponentsTurn() : Some other type";
        return true;
        break;
    }
    }
}

void GameManager::loadBreeds(Message<GameMsgTypes> &msg)
{
    vector<Breed> breeds;
    BOOST_LOG_TRIVIAL(info) << "GameManager::loadBreeds() : Trying to load breed!";
    msg >> breeds;
    BOOST_LOG_TRIVIAL(info) << "GameManager::loadBreeds() : breed loaded!";

    loadHeroesStats(playerUnits, breeds, playerTilesManager, side);
    loadHeroesStats(opponentUnits, breeds, opponentTilesManager, (1 - side));

    playerTilesManager.updateFocus();
    opponentTilesManager.updateFocus();
    buttonsManager.updateFocus();
}

void GameManager::_whileForPlay()
{
    while (window.isOpen() && stage != GameStage::stageWon && stage != GameStage::stageLost)
    {
        switch (stage)
        {
        case (GameStage::stageOpponentsTurn):
            stageText.setRound(Round::opponentsTurn);
            stageText.restart();
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
    if (stage == GameStage::stageLost)
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::play(): You lost!";
        return;
    }
    else if (stage == GameStage::stageWon)
    {
        BOOST_LOG_TRIVIAL(info) << "GameManager::play(): You won!";
        return;
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
                background.updateFocus();
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
                background.updateFocus();
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
            if (!handleMessageFromOpponent())
            {
                return;
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

void GameManager::_whileForPlayersTurn()
{
    int movesAmount = 0;
    buttonsManager.disable();
    if (playerTilesManager.hasBodies())
    {
        btnRemoveBody->enable();
    }
    if (cardsManager.canTakeCard())
    {
        btnTakeCard->enable();
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
                background.updateFocus();
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
                if (cardsManager.getCardWasTaken() && btnTakeCard->hasFocus() || playerTilesManager.mouseClicked())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Card was taken!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    playerTilesManager.setActiveRoundTiles();
                    if (!cardsManager.canTakeCard())
                    {
                        btnTakeCard->disable();
                    }
                    else
                    {
                        btnTakeCard->enable();
                    }
                }

                if (playerTilesManager.hasTileBuffer() == false)
                {
                    btnAttack->disable();
                    btnPower->disable();
                }
                else
                {
                    btnAttack->enable();
                    btnPower->enable();
                }

                //Если атаковали противника
                if (opponentTilesManager.mouseClicked() ||
                    playerTilesManager.powerWasUsed() ||
                    opponentTilesManager.powerWasUsed() ||
                    playerTilesManager.removedBody())
                {
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : something was done!";
                    movesAmount++;
                    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn() : Moves count = " << movesAmount;
                    if (!handleMessage(GameMsgTypes::GameHeroesStats))
                    {
                        return;
                    }
                    if (!playerTilesManager.hasBodies())
                    {
                        btnRemoveBody->disable();
                    }
                    else
                    {
                        btnRemoveBody->enable();
                    }
                    if (!cardsManager.canTakeCard())
                    {
                        btnTakeCard->disable();
                    }
                    else
                    {
                        btnTakeCard->enable();
                    }
                    playerTilesManager.setActiveRoundTiles();
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    opponentTilesManager.resetActiveTiles();
                    opponentTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
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

GameManager::~GameManager()
{
    btnAttack = nullptr;
    btnPower = nullptr;
    btnTakeCard = nullptr;
    btnRemoveBody = nullptr;
}