#include "Command.h"
#include "Tile.h"

class CommandAttack : public Command
{
    public:
        CommandAttack(TilesManager* _playerTM, TilesManager* _opponentTM);
        void execute() override;
        ~CommandAttack();

    private:
        TilesManager* playerTilesManager;
        TilesManager* opponentTilesManager;

};
