#include "Command.h"
#include "Tile.h"
#include "GameTcpClient.h"
#include "GameMsgTypes.h"

class CommandAttack : public Command
{
public:
    CommandAttack(TilesManager &_playerTM, TilesManager &_opponentTM, GameTcpClient &_client);
    void execute() override;
    ~CommandAttack();

private:
    GameTcpClient &client;
    TilesManager &playerTilesManager;
    TilesManager &opponentTilesManager;
};
