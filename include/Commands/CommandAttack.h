#include "Command.h"
#include "Tile.h"

class CommandAttack : public Command
{
    public:
        CommandAttack(TilesManager* _tilesManager);
        void execute() override;
        ~CommandAttack();

    private:
        TilesManager* tilesManager;
        Tile* attackerTile;
        Tile* attackedTile;
};
