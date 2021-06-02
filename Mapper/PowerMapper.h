#ifndef DB_POWERMAPPER_H
#define DB_POWERMAPPER_H

#include "../Game/Powers.h"

using std::vector;

class PowerMapper {

private:
    Powers *powers;
public:
    PowerMapper();

    void MapPowers(Hero &hero);
};

#endif //DB_POWERMAPPER_H
