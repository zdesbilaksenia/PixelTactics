#include "PowerMapper.h"

void PowerMapper::MapPowers(Hero &hero) {
    hero.frontLinePower = powers->ReturnFrontLinePower(hero.GetName());
    hero.middleLinePower = powers->ReturnMiddleLinePower(hero.GetName());
    hero.backLinePower = powers->ReturnMiddleLinePower(hero.GetName());
}
