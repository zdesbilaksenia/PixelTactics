#include "../DrawableEntities/Tile.h"

class Command
{
public:
    Command();
    virtual void Execute() = 0;
    ~Command();
};

class CommandTakeCard : public Command
{
public:
    CommandTakeCard();
    void Execute() override;
    ~CommandTakeCard();
};

class CommandReleaseHero : public Command
{
public:
    CommandReleaseHero();
    CommandReleaseHero(int _characterId, int _mapX, int _mapY);
    void Execute() override;
    ~CommandReleaseHero();

private:
    int characterId;
    int mapX;
    int mapY;
};

class CommandAttack : public Command
{
public:
    CommandAttack();
    CommandAttack(int _atackerId, Tile &_attackedTile);
    void Execute() override;
    ~CommandAttack();

private:
    int attackerId ;
    Tile attackedTile;
};

class CommandUseMagic : public Command
{
public:
    CommandUseMagic();
    CommandUseMagic(int _atackerId, Tile &_attackedTile);
    void Execute() override;
    ~CommandUseMagic();

private:
    int attackerId;
    Tile attackedTile;
};
