#include "Command.h"
#include "Tile.h"
#include "GameTcpClient.h"
#include "GameMsgTypes.h"

class CommandPower : public Command
{
public:
    CommandPower(TilesManager &_playerTM, TilesManager &_opponentTM, GameTcpClient &_client);
    void execute() override;
    ~CommandPower();

private:
    GameTcpClient &client;
    TilesManager &playerTilesManager;
    TilesManager &opponentTilesManager;
};