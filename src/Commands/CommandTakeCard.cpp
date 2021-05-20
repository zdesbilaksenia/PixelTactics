#include "Commands/CommandTakeCard.h"

#include <iostream>
using namespace std;

CommandTakeCard::CommandTakeCard(CardsManager &_cardsManager) : cardsManager(_cardsManager)
{
}

void CommandTakeCard::execute()
{
    ////////////////////////////////////////////////////////////////////////////////////////COUT
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