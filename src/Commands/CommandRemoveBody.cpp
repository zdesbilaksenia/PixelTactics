#include "Commands/CommandRemoveBody.h"

CommandRemoveBody::CommandRemoveBody(TilesManager &_tilesManager, GameTcpClient &_client) : playerTilesManager(_tilesManager), client(_client)
{
}

void CommandRemoveBody::execute()
{
    BOOST_LOG_TRIVIAL(info) << "CommandRemoveBody::execute(): start!";
    client.sendRemoveBodyRequest();

    client.incoming().wait();
    auto msg = client.incoming().popFront().msg;
    vector<bool> activeTiles;
    switch (msg.header.id)
    {
    case GameMsgTypes::GameRemoveBody:
    {
        BOOST_LOG_TRIVIAL(info) << "CommandRemoveBody::execute(): Trying to load active tiles!";
        msg >> activeTiles;
        cout<<endl<<"size = "<<activeTiles.size()<<endl;
        for(int i = 0; i<9; ++i)
        {
            cout<<activeTiles[i]<<" ";
        }
        cout<<endl;
        BOOST_LOG_TRIVIAL(info) << "CommandRemoveBody::execute(): active tiles loaded!";
        playerTilesManager.setActiveTiles(activeTiles);
        //Сама обработка запроса будет происходить уже в buttonIsPressed в объекте opponentTilesManager.
        playerTilesManager.setStatus(TilesManagerStatus::statusWaitingForRemovingBody);
        break;
    }
    case GameMsgTypes::GameReject:
    {
        BOOST_LOG_TRIVIAL(info) << "CommandRemoveBody::execute(): Can't attack!";
        return;
    }
    default:
    {
        cout << msg << endl;
        cout << "OTHER" << endl;
        return;
    }
    }

    //playerTilesManager.updateFocus();
}

CommandRemoveBody::~CommandRemoveBody()
{
}