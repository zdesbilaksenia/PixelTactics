#ifndef PIXELTACTICS_DATABASEBUILDER_H
#define PIXELTACTICS_DATABASEBUILDER_H

#include "MySQL.h"
#include "Connection.h"
#include <memory>

class DataBaseBuilder {

private:
    std::unique_ptr<MySQL> dataBase = nullptr;
    std::unique_ptr<Connection> connection = nullptr;
public:
    DataBaseBuilder() {};

    void BuildConnection() {};

    void BuildMySQL() {};

    std::unique_ptr<MySQL> GetResult() {};
};

#endif //PIXELTACTICS_DATABASEBUILDER_H
