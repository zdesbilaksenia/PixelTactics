//То же самое (почти), что и CommandSetUnit!!!
//Реализация для карт (которые в руках у игрока)

#include "Command.h"
#include "Unit.h"
#include "Tile.h"
#include "Card.h"

//Комманда, видимо, вообще не пригодится

class CommandReleaseUnit : public Command
{
    public:
        CommandReleaseUnit(TilesManager& _tilesManager);

        void execute() override;
        ~CommandReleaseUnit();

    private:
        TilesManager& tilesManager;
};