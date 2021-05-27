#ifndef PIXELTACTICS_MYSQL_H
#define PIXELTACTICS_MYSQL_H

#include "Connection.h"
#include "Card.h"
#include <memory>
#include <mysql.h>
#include <vector>

using std::vector;
using std::unique_ptr;
using std::make_unique;
using std::move;

class MySQL {

private:
    unique_ptr<Connection> connection;
public:
    MySQL();

    vector<Card> GetCards();
};

#endif //PIXELTACTICS_MYSQL_H
