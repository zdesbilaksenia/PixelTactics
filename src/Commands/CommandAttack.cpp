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
                opponentTilesManager->setStatus(TilesManagerStatus::statusWaitingForAttack);
                //opponentTilesManager->setTileBuffer(playerTilesManager->getTileBuffer());

                playerTilesManager->setTileBuffer(nullptr);
                playerTilesManager->setStatus(TilesManagerStatus::statusNothingHappens);
                playerTilesManager->updateFocus();
                opponentTilesManager->updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}