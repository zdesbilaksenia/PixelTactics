#ifndef PIXELTACTICS_DATABASEDIRECTOR_H
#define PIXELTACTICS_DATABASEDIRECTOR_H

#include "DataBaseBuilder.h"

class DataBaseDirector {

private:
    DataBaseBuilder *builder = nullptr;
public:

    void SetBuilder(DataBaseBuilder *_builder) {};

    bool IsBuilderSet() {};

    MySQL *MakeDataBase() {};
};

#endif //PIXELTACTICS_DATABASEDIRECTOR_H
