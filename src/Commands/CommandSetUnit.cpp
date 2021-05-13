#include "Commands/CommandSetUnit.h"
#include "Tile.h"

// ====================================
// ==========CommandSetUnit============
// ====================================
CommandSetUnit::CommandSetUnit(Unit* _unit, TilesManager& _tilesManager) : unit(_unit), tilesManager(_tilesManager) {}

void CommandSetUnit::execute()
{
    tilesManager.setStatus(TilesManagerStatus::statusReleasingUnit);
    tilesManager.setUnitBuffer(this->unit);
}

CommandSetUnit::~CommandSetUnit() {}