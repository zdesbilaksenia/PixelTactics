#include "CardsMapper.h"

void CardsMapper::MapCard(Card &card) {

    card.frontLinePower = powers->ReturnFrontLinePower(card.ID - 1);
    card.middleLinePower = powers->ReturnMiddleLinePower(card.ID - 1);
    card.backLinePower = powers->ReturnBackLinePower(card.ID - 1);
    card.leaderPower = powers->ReturnLeaderPower(card.ID - 1);
}
