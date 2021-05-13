#include "Commands/CommandReleaseUnit.h"

// ====================================
// ========CommandReleaseUnit==========
// ====================================
CommandReleaseUnit::CommandReleaseUnit(Unit* _unit, TilesManager& _tilesManager) : unit(_unit), tilesManager(_tilesManager) {}

void CommandReleaseUnit::execute()
{
    tilesManager.setStatus(statusReleasingUnit);
    tilesManager.setUnitBuffer(this->unit);
    //this->unit = nullptr;
}

CommandReleaseUnit::~CommandReleaseUnit() {}