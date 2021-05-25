#include "Commands/CommandTakeCard.h"

#include <iostream>
using namespace std;

CommandTakeCard::CommandTakeCard(CardsManager &_cardsManager, GameTcpClient& _client) : cardsManager(_cardsManager), client(_client)
{
}

void CommandTakeCard::execute()
{
    //Доделать отправку серверу инфы о взятии карты
    if (cardsManager.takeCard())
    {
        cout << "CommandTakeCard::execute():: Card was taken!!!" << endl;
    }
    else
    {
        cout << "CommandTakeCard::execute():: Card wasn't taken!!!" << endl;
    }
}

CommandTakeCard::~CommandTakeCard()
{
}