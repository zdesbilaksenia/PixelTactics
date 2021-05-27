#include "Commands/CommandPower.h"

#include "iostream"
using namespace std;
// ====================================
// ===========CommandPower============
// ====================================

CommandPower::CommandPower(TilesManager &_playerTM, TilesManager &_opponentTM, GameTcpClient &_client) : playerTilesManager(_playerTM),
                                                                                                         opponentTilesManager(_opponentTM),
                                                                                                         client(_client)
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
            return;
        }
        case GameMsgTypes::GameBeforeHeroesStats:
        {
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

            for (int i = 0; i < 9; ++i)
            {
                cout << activeTiles[i] << " ";
            }
            cout << endl;

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
            opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForPower);

            break;
        }
        default:
            cout << "Other type" << endl;
            break;
        }

        //Здесь это, вроде не нужно, нужно менять потом.
        playerTilesManager.setTileBuffer(nullptr);
        //playerTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
        playerTilesManager.updateFocus();
        opponentTilesManager.updateFocus();
    }
}

CommandPower::~CommandPower()
{
}