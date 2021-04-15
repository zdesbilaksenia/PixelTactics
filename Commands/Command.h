#include "../CommandManager/CommandManager.h"
#include "../DrawableEntities/Tile.h"

class Command
{
public:
    Command();
    void setCommandManager(CommandManager &_cmdManager);
    virtual void Execute() = 0;
    ~Command();

private:
    CommandManager comandManager;
}

class CommandTakeCard : public Command
{
public:
    CommandTakeCard();
    void Execute() override;
    ~CommandTakeCard();
}

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
}

class CommandAttack : public Command
{
public:
    CommandAttack();
    CommandAttack(int _atackerId, Tile &_attackedTile) void Execute() override;
    ~CommandAttack();

private:
    attackerId : int;
    attackedTile : Tile &;
}

class CommandUseMagic : public Command
{
public:
    CommandUseMagic();
    CommandUseMagic(int _atackerId, Tile &_attackedTile);
    void Execute() override;
    ~CommandUseMagic();

private:
    attackerId : int;
    attackedTile : Tile &;
}
