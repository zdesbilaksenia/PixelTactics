#include "Commands/CommandAttack.h"

#include "iostream"
using namespace std;
// ====================================
// ===========CommandAttack============
// ====================================

CommandAttack::CommandAttack(TilesManager *_playerTM, TilesManager *_opponentTM) : playerTilesManager(_playerTM), opponentTilesManager(_opponentTM)
{
}

void CommandAttack::execute()
{
        if (playerTilesManager->getTileBuffer() == nullptr)
        {
                cout << "CommandAttack::execute(): ERROR playerTileBuffer is empty!" << endl;
        }
        else
        {
                cout << "CommandAttack::execute(): Attacking!" << endl;

                //playerTilesManager->setStatus(TilesManagerStatus::statusNothingHappens);
                opponentTilesManager->setStatus(TilesManagerStatus::statusWaitingForAttack);
                opponentTilesManager->setTileBuffer(playerTilesManager->getTileBuffer());
                playerTilesManager->setTileBuffer(nullptr);
                //playerTilesManager->updateFocus();
                opponentTilesManager->updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}