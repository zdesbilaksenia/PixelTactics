#ifndef PIXELTACTICS_CARDSMAPPER_H
#define PIXELTACTICS_CARDSMAPPER_H

#include "Connection.h"
#include "../../Cards/Powers.h"
#include "../../Cards/Card.h"
#include <memory>

using std::vector;

class CardsMapper {

private:
    Powers powers;
    std::shared_ptr<Connection> connection;
public:
    CardsMapper(Connection &_connection) {};

    vector<Card> MapCards() {};
};

#endif //PIXELTACTICS_CARDSMAPPER_H
