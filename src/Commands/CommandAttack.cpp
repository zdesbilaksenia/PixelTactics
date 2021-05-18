#include "Commands/CommandAttack.h"

// ====================================
// ===========CommandAttack============
// ====================================

CommandAttack::CommandAttack(TilesManager *_tilesManager)
{

}

void CommandAttack::execute()
{

}

CommandAttack::~CommandAttack()
{
        tilesManager = nullptr;
        attackerTile = nullptr;
        attackedTile = nullptr;
}