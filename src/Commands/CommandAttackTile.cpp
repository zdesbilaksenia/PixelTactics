#include "Commands/CommandAttackTile.h"

// ====================================
// =========CommandAttackTile==========
// ====================================
CommandAttackTile::CommandAttackTile(vector<Tile *> &_tiles) : tiles(_tiles) {}

void CommandAttackTile::execute()
{
    for (auto tile : this->tiles)
    {
        if (tile->getStatus() == TileStatus::statusHasUnit)
        {
            tile->setFillColor(Color::Yellow);
        }
    }
}

CommandAttackTile::~CommandAttackTile() {}