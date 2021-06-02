#include "Commands/CommandPower.h"

#include "iostream"
using std::cout;
using std::endl;
// ====================================
// ===========CommandPower============
// ====================================

CommandPower::CommandPower(TilesManager &_playerTM, TilesManager &_opponentTM, GameTcpClient &_client) : client(_client),
                                                                                                         playerTilesManager(_playerTM),
                                                                                                         opponentTilesManager(_opponentTM)

{
}

void CommandPower::execute()
{
    if (playerTilesManager.getTileBuffer() == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "CommandPower::execute(): ERROR playerTileBuffer is empty!";
        return;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "CommandPower::execute(): Using power!";
        cout << "X = " << playerTilesManager.getTileBuffer()->getCoordX() << " Y = " << playerTilesManager.getTileBuffer()->getCoordY() << endl;

        client.sendPowerUserPos(playerTilesManager.getTileBuffer()->getCoordX(), playerTilesManager.getTileBuffer()->getCoordY());

        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        switch (msg.header.id)
        {
        case GameMsgTypes::GameReject:
        {
            BOOST_LOG_TRIVIAL(info) << "CommandPower::execute(): Can't use power!";
            playerTilesManager.setTileBuffer(nullptr);
            return;
        }
        case GameMsgTypes::GameBeforeHeroesStats:
        {
            BOOST_LOG_TRIVIAL(info) << "CommandPower::execute(): got message GameBeforeHeroesStats!";
            opponentTilesManager.setStatus(TilesManagerStatus::statusWhenThePowerWhichChangesStatsImmidiatelyWasActivated);
            break;
        }
        case GameMsgTypes::GamePowerAnswer:
        {
            opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForAttack);
            opponentTilesManager.setTileBuffer(playerTilesManager.getTileBuffer());
            vector<bool> activeTiles;
            BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Trying to load active tiles!";
            msg >> activeTiles;
            BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): active tiles loaded!";

            vector<bool> playerActiveTiles;
            vector<bool> opponentActiveTiles;

            bool side = client.getSide();
            for (int i = 0 + 9 * side; i < 9 + 9 * side; ++i)
            {
                playerActiveTiles.push_back(activeTiles[i]);
            }
            for (int i = 0 + 9 * (1 - side); i < 9 + 9 * (1 - side); ++i)
            {
                opponentActiveTiles.push_back(activeTiles[i]);
            }

            opponentTilesManager.setActiveTiles(opponentActiveTiles);
            playerTilesManager.setActiveTiles(playerActiveTiles);

            playerTilesManager.setStatus(TilesManagerStatus::statusWaitingForPower);
            opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForPower);

            break;
        }
        default:
            BOOST_LOG_TRIVIAL(error) << "CommandAttack::execute(): got other type of message: " << msg;
            playerTilesManager.setTileBuffer(nullptr);
            return;
            break;
        }

        playerTilesManager.setTileBuffer(nullptr);
        playerTilesManager.updateFocus();
        opponentTilesManager.updateFocus();
    }
}

CommandPower::~CommandPower()
{
}