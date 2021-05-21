#include "Commands/CommandAttack.h"

#include "iostream"
using namespace std;
// ====================================
// ===========CommandAttack============
// ====================================

CommandAttack::CommandAttack(TilesManager *_playerTM, TilesManager *_opponentTM) : playerTilesManager(_playerTM), opponentTilesManager(_opponentTM)
{
}

void CommandAttack::execute()
{  
        if (playerTilesManager->getTileBuffer() == nullptr)
        {
                cout << "CommandAttack::execute(): ERROR playerTileBuffer is empty!" << endl;
        }
        else
        {
                //На сервер запрос Действие::Аттака
                //client->sendMessage(Action::Attack);
                opponentTilesManager->setStatus(TilesManagerStatus::statusWaitingForAttack);
                //Сделать метод у менеджеров, который будет ждать ответа от сервера, и в зависимости от ответа (в котором будет указан тип ответа)
                //будет что-то делать
                //client->waitForAnswer();
                //На клиент приходит сообщение "ActionToClient::SetDrawableTiles" и массив 3 на 3 с тем, что можно отриовать.
                //И в этом же методе клиент вызывает у opponentTilesManager метод, обновляющий этот массив у менеджера.

                //cout << "CommandAttack::execute(): Attacking!" << endl;

                opponentTilesManager->setStatus(TilesManagerStatus::statusWaitingForAttack);
                //Почему закомментил - не помню...
                //opponentTilesManager->setTileBuffer(playerTilesManager->getTileBuffer());

                playerTilesManager->setTileBuffer(nullptr);
                playerTilesManager->setStatus(TilesManagerStatus::statusNothingHappens);
                playerTilesManager->updateFocus();
                opponentTilesManager->updateFocus();
        }
}

CommandAttack::~CommandAttack()
{
}