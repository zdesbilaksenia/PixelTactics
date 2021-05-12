#include "Command.h"
#include "Unit.h"
#include "Tile.h"

//
//==============================================================
//Может вообще не нужен класс Unit?
//В тайле будут храниться значения характеристик персонажа и всё
//==============================================================
//
//Поставить юнита на тайл (союзный).
//Комманда предназначена для карт.
//
class CommandSetUnit : public Command
{
    public:
        CommandSetUnit(Unit* _unit, TilesManager& _tilesManager);
        void execute() override;
        ~CommandSetUnit();

    private:
        Unit* unit;
        TilesManager& tilesManager;
};