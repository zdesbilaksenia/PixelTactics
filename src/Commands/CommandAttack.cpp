#include "Commands/CommandAttack.h"

#include "iostream"
using namespace std;
// ====================================
// ===========CommandAttack============
// ====================================

CommandAttack::CommandAttack(TilesManager &_playerTM, TilesManager &_opponentTM) : playerTilesManager(_playerTM),
                                                                                   opponentTilesManager(_opponentTM)
{
}

void CommandAttack::execute()
{
        if (playerTilesManager.getTileBuffer() == nullptr)
        {
                cout << "CommandAttack::execute(): ERROR playerTileBuffer is empty!" << endl;
                return;
        }
        else
        {
                cout << "CommandAttack::execute(): Attacking!" << endl;
                //Сама обработка запроса будет происходить уже в buttonIsPressed в объекте opponentTilesManager.
                opponentTilesManager.setTileBuffer(playerTilesManager.getTileBuffer());
                opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForAttack);
                opponentTilesManager.updateFocus();

                //Здесь должны получить массив bool
                //vector<bool> activeTiles = message.getVector(); или типа того
                vector<bool> activeTiles = {1, 1, 1, 1, 1, 1, 1, 1, 1};
                bool opponentActiveTiles[3][3] = {
                    {activeTiles[0], activeTiles[1], activeTiles[2]},
                    {activeTiles[3], activeTiles[4], activeTiles[5]},
                    {activeTiles[6], activeTiles[7], activeTiles[8]},
                };
                opponentTilesManager.setActiveTiles(opponentActiveTiles);
                opponentTilesManager.updateFocus();

                                                 playerTilesManager.setTileBuffer(nullptr);
                playerTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                playerTilesManager.updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}