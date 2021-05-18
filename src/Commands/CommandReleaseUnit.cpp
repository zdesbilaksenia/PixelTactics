#include "Commands/CommandReleaseUnit.h"
#include <iostream>
using namespace std;

// ====================================
// ========CommandReleaseUnit==========
// ====================================
CommandReleaseUnit::CommandReleaseUnit(TilesManager &_tilesManager) : tilesManager(_tilesManager) {}

void CommandReleaseUnit::execute()
{
    tilesManager.setStatus(TilesManagerStatus::statusReleasingUnit);
}

CommandReleaseUnit::~CommandReleaseUnit() {}