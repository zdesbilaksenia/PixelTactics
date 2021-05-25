#include "Commands/CommandAttack.h"

#include "iostream"
using namespace std;
// ====================================
// ===========CommandAttack============
// ====================================

CommandAttack::CommandAttack(TilesManager &_playerTM, TilesManager &_opponentTM, GameTcpClient &_client) : playerTilesManager(_playerTM),
                                                                                                           opponentTilesManager(_opponentTM),
                                                                                                           client(_client)
{
}

void CommandAttack::execute()
{
        if (playerTilesManager.getTileBuffer() == nullptr)
        {
                BOOST_LOG_TRIVIAL(error) << "CommandAttack::execute(): ERROR playerTileBuffer is empty!";
                return;
        }
        else
        {
                BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Attacking!";
                //Сама обработка запроса будет происходить уже в buttonIsPressed в объекте opponentTilesManager.
                opponentTilesManager.setTileBuffer(playerTilesManager.getTileBuffer());
                opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForAttack);
                opponentTilesManager.updateFocus();

                BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Sending attacker position!";
                client.sendAttackerPos(playerTilesManager.getTileBuffer()->getCoordX(), playerTilesManager.getTileBuffer()->getCoordY());
                BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Attacker position sent!";

                client.incoming().wait();
                auto msg = client.incoming().popFront().msg;
                vector<bool> activeTiles;
                if (msg.header.id == GameMsgTypes::GameCanBeAttacked)
                {
                        BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Trying to load active tiles!";
                        msg >> activeTiles;
                        BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): active tiles loaded!";
                }

#if SERVER_CONNECTING == 0
                //vector<bool> activeTiles = {1, 1, 1, 1, 1, 1, 1, 1, 1};
#endif // SERVER_CONNECTING

                // 1 1 1
                // 0 1 0
                // 1 1 0
                // ============ 1 0 0 1 1 1 1 0 1
                // ============ 1 0 1 1 1 1 1 0 1

                for (int i = 0; i < 9; ++i)
                {
                        cout << activeTiles[i] << " ";
                }
                cout << endl;

                opponentTilesManager.setActiveTiles(activeTiles);

                opponentTilesManager.updateFocus();

                playerTilesManager.setTileBuffer(nullptr);
                playerTilesManager.setStatus(TilesManagerStatus::statusNothingHappens);
                playerTilesManager.updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}