#include "GameManager.h"
#include "configurations.cpp"

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
    round = RoundType::roundNoType;
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
}

void GameManager::playersTurn()
{
    playerTilesManager.setTileBuffer(nullptr);
    opponentTilesManager.setTileBuffer(nullptr);

    BOOST_LOG_TRIVIAL(info) << "GameManager::playersTurn(): Started!";

    playerTilesManager.setRound(round);
    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);

    playerTilesManager.enable();
    playerTilesManager.updateFocus();

    opponentTilesManager.enable();
    opponentTilesManager.updateFocus();

    buttonsManager.enable();
    buttonsManager.updateFocus();

    _whileForPlayersTurn();

    if (stage == GameStage::stageWon)
    {
        return;
    }
}

GameManager::~GameManager()
{

    btnAttack = nullptr;
    btnPower = nullptr;
    btnCancel = nullptr;
    btnTakeCard = nullptr;
    btnRemoveBody = nullptr;
}