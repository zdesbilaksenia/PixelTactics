#ifndef DB_POWERMAPPER_H
#define DB_POWERMAPPER_H

#include "Powers.h"

using std::vector;

class PowerMapper {

private:
    Powers *powers;
public:
    PowerMapper() : powers(new Powers()) {};

    void MapPowers(Hero &hero);
};

#endif //DB_POWERMAPPER_H
