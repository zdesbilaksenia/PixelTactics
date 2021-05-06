#ifndef PIXELTACTICS_CONNECTION_H
#define PIXELTACTICS_CONNECTION_H

#include <string>
#include <iostream>
#include "ConnectionDetails.h"
#include "mysql.h"

using std::cout;
using std::string;

class Connection {

private:
    MYSQL *connection;
    ConnectionDetails details;
public:
    Connection(ConnectionDetails &_details) : details(_details), connection(mysql_init(new MYSQL())) {};

    bool MySqlConnectionSetup();

    MYSQL_RES *ExecuteQuery(const string &query);
};

#endif //PIXELTACTICS_CONNECTION_H
