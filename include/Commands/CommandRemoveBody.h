#include "Command.h"
#include "Tile.h"

class CommandRemoveBody : public Command
{
public:
    CommandRemoveBody(TilesManager &_tilesManager, GameTcpClient &_client);

    void execute() override;
    ~CommandRemoveBody();

private:
    TilesManager &playerTilesManager;
    GameTcpClient &client;
};