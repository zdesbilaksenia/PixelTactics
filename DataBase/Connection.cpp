#include "Connection.h"

bool Connection::MySqlConnectionSetup() {

    if (!mysql_real_connect(move(connection).get(), details.server.c_str(), details.user.c_str(), details.password.c_str(),
                            details.dataBase.c_str(), 3306, nullptr,
                            0)) {
        cout << "ERROR DB CONNECTING\n";
        return false;
    }
    return true;
}

MYSQL_RES *Connection::ExecuteQuery(const string &query) {

    mysql_select_db(move(connection).get(), "Pixel Tactics");
    if (mysql_query(move(connection).get(), query.c_str())) {
        cout << "ERROR QUERY\n";
        return nullptr;
    }

    return mysql_use_result(move(connection).get());
}

