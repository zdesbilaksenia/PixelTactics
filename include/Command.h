#pragma once

#include<string>
using std::string;
#include <boost/log/trivial.hpp>
using namespace boost::log;


class Command
{
public:
    Command();
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


//Тестовая команда
class CommandStringTest : public Command
{
    public:
        CommandStringTest(string _str);
        void execute() override;
        ~CommandStringTest();
    private:
        string str;
};
