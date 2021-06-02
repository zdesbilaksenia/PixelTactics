#include "Commands/CommandAttack.h"

#include "iostream"
using std::cout;
using std::endl;
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
                BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Sending attacker position!";
                cout << " ATTACKER POS = " << playerTilesManager.getTileBuffer()->getCoordX() << " " << playerTilesManager.getTileBuffer()->getCoordY() << endl;
                client.sendAttackerPos(playerTilesManager.getTileBuffer()->getCoordX(), playerTilesManager.getTileBuffer()->getCoordY());
                BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Attacker position sent!";

                client.incoming().wait();
                auto msg = client.incoming().popFront().msg;
                vector<bool> activeTiles;
                switch (msg.header.id)
                {
                case GameMsgTypes::GameCanBeAttacked:
                {
                        BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Trying to load active tiles!";
                        msg >> activeTiles;
                        BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): active tiles loaded!";
                        opponentTilesManager.setActiveTiles(activeTiles);
                        //Сама обработка запроса будет происходить уже в buttonIsPressed в объекте opponentTilesManager.
                        opponentTilesManager.setTileBuffer(playerTilesManager.getTileBuffer());
                        opponentTilesManager.setStatus(TilesManagerStatus::statusWaitingForAttack);
                        break;
                }
                case GameMsgTypes::GameReject:
                {
                        BOOST_LOG_TRIVIAL(info) << "CommandAttack::execute(): Can't attack!";
                        playerTilesManager.setTileBuffer(nullptr);
                        return;
                }
                }

                playerTilesManager.setTileBuffer(nullptr);
                playerTilesManager.updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}