#ifndef PIXELTACTICS_MYSQL_H
#define PIXELTACTICS_MYSQL_H

#include "CardsMapper.h"
#include "Connection.h"
#include "../../Cards/Card.h"
#include <memory>

using std::vector;

class MySQL {

private:
    std::unique_ptr<CardsMapper> mapper;
public:
    MySQL(Connection connection) {};

    vector<Card> GetCards() {};
};

#endif //PIXELTACTICS_MYSQL_H
