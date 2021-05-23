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
    cout << "GameManager::opponentsTurn(): Started!" << endl;

    playerTilesManager.disable();
    buttonsManager.disable();

    _whileForOpponentsTurn();
}

void GameManager::playersTurn()
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

    _whileForPlayersTurn();
}