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
                         vector<Unit> &_opponentUnits,
                         const vector<Texture> &_textures) : window(_window),
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
                                                             opponentUnits(_opponentUnits),
                                                             unitTextures(_textures)
{
}

void GameManager::setAttackButton(Button &_btnAttack)
{
    btnAttack = make_unique<Button>(_btnAttack);
}

void GameManager::setPowerButton(Button &_btnPower)
{
    btnPower = make_unique<Button>(_btnPower);
}

void GameManager::setCancelButton(Button &_btnCancel)
{
    btnCancel = make_unique<Button>(_btnCancel);
}

void GameManager::setTakeCardButton(Button &_btnTakeCard)
{
    btnTakeCard = make_unique<Button>(_btnTakeCard);
}

void GameManager::setRemoveBodyButton(Button &_btnRemoveBody)
{
    btnRemoveBody = make_unique<Button>(_btnRemoveBody);
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
    //stage = GameStage::stageOpponentsTurn;
    stage = GameStage::stagePlayersTurn;

    //Ждём здесь, пока ответ о конце хода противника не придёт:
    opponentsTurn();

    round = RoundType::roundAvangard;

    _whileForPlay();
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

    //Вначале игры берутся 3 карты
    for (int i = 0; i < 3; ++i)
    {
        cardsManager.takeCard();
    }

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
}