#include "Commands/CommandTakeCard.h"

#include <iostream>
using std::cout;
using std::endl;

CommandTakeCard::CommandTakeCard(CardsManager &_cardsManager, GameTcpClient& _client) : cardsManager(_cardsManager), client(_client)
{
}

void CommandTakeCard::execute()
{
    //Доделать отправку серверу инфы о взятии карты
    if (cardsManager.canTakeCard())
    {
        cardsManager.takeCard();
        BOOST_LOG_TRIVIAL(info) << "CommandTakeCard::execute():: Card was taken!";
        client.sendTakeCard();
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "CommandTakeCard::execute():: Card wasn't taken!";
    }
}

CommandTakeCard::~CommandTakeCard()
{
}