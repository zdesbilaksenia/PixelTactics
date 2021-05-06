#ifndef PIXELTACTICS_CARDSMAPPER_H
#define PIXELTACTICS_CARDSMAPPER_H

#include "../Connection.h"
#include "../../Cards/Card.h"
#include <memory>

using std::vector;

class CardsMapper {

private:
    Powers *powers;
public:
    CardsMapper() : powers(new Powers()) {};

    void MapCard(Card &card);
};

#endif //PIXELTACTICS_CARDSMAPPER_H
