#ifndef PIXELTACTICS_CONNECTION_H
#define PIXELTACTICS_CONNECTION_H

#include "ConnectionDetails.h"

using std::string;

class Connection {

private:
    ConnectionDetails details;
public:

    Connection(ConnectionDetails _details) {};

    //bool InitDataBase();

    bool MySqlConnectionSetup() {};

    bool ExecuteQuery(string query) {};
};

#endif //PIXELTACTICS_CONNECTION_H
