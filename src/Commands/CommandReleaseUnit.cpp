#include "Commands/CommandReleaseUnit.h"
#include <iostream>
using namespace std;

// ====================================
// ========CommandReleaseUnit==========
// ====================================
CommandReleaseUnit::CommandReleaseUnit(TilesManager &_tilesManager) : tilesManager(_tilesManager) {}

void CommandReleaseUnit::execute()
{
    ///////////////////////////////////////////////////////////////////COUT
    cout << "HEY" << endl;
    //То есть мы забираем юнита из буффера обмена tilesManager и сохраняем его в комманду.
    unit = tilesManager.getUnitBuffer();
    cout << unit << endl;

    tilesManager.setStatus(TilesManagerStatus::statusReleasingUnit);
    tilesManager.setUnitBuffer(this->unit);
    //this->unit = nullptr;
}

CommandReleaseUnit::~CommandReleaseUnit() {}