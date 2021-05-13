//То же самое (почти), что и CommandSetUnit!!!
//Реализация для карт (которые в руках у игрока)

#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include "Card.h"

class CommandReleaseUnit : public Command
{
    public:
        CommandReleaseUnit(Unit* _unit, TilesManager& _tilesManager);
        
        void execute() override;
        ~CommandReleaseUnit();

    private:
        Unit* unit;
        TilesManager& tilesManager;
};