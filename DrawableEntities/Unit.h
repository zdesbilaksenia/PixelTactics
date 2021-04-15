#include "DrawableBox.h"
#include "Label.h"

class Unit : public DrawableBox
{
public:
    Unit();
    Unit(int characterId);
    void setHealth(int _health);
    void setAttack(int _attack);
    int getHealth();
    int getAttack();
    int getCharacterId();
    ~Unit();

private:
    int characterId;
    int health;
    int attack;
    Label healthLabel;
    Label attackLabel;
};