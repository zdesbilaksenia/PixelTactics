#pragma once

//#include "../CommandManager/CommandManager.h"
//#include "../DrawableEntities/Tile.h"
#include<string>
using std::string;
//#include "Tile.h"


class Command
{
public:
    Command();
    //void setCommandManager(CommandManager &_cmdManager);
    virtual void execute() = 0;
    ~Command();

private:
    //CommandManager comandManager;
};

class CommandTest : public Command
{
    public:
        CommandTest();
        void execute() override;
        ~CommandTest();
};

class CommandStringTest : public Command
{
    public:
        CommandStringTest(string _str);
        void execute() override;
        ~CommandStringTest();
    private:
        string str;
};
