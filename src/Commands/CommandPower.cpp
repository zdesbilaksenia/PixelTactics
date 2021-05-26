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
        BOOST_LOG_TRIVIAL(info) << "CommandPower::execute(): Attacking!";
        //Сделать statusWaitingForPower (чтобы другими цветами рисовать)
        opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForAttack);

        client.sendPowerUserPos(playerTilesManager.getTileBuffer()->getCoordX(), playerTilesManager.getTileBuffer()->getCoordY());

        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        switch (msg.header.id)
        {
        case GameMsgTypes::GameHeroesStats:
        {

            break;
        }
        case GameMsgTypes::GamePowerAnswer:
        {
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

            opponentTilesManager.setActiveTiles(activeTiles);

            break;
        }
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