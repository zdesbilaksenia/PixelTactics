#ifndef PIXELTACTICS_MYSQL_H
#define PIXELTACTICS_MYSQL_H

#include "CardsMapper.h"
#include "Connection.h"
#include "Card.h"
#include <memory>
#include <mysql.h>
#include <vector>

using std::vector;

class MySQL {

private:
    Connection *connection;
    CardsMapper *mapper;
public:
    MySQL();

    vector<Card> GetCards();
};

#endif //PIXELTACTICS_MYSQL_H
