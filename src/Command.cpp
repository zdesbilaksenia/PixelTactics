#include "../include/Command.h"
#include <iostream>
using std::cout;
using std::endl;

Command::Command() {}

Command::~Command() {}

// ====================================
// ============CommandTest=============
// ====================================
CommandTest::CommandTest() {}

CommandTest::~CommandTest() {}

void CommandTest::execute()
{
    cout << "CommandTest::execute()" << endl;
}
// ====================================
// =========CommandStringTest==========
// ====================================
CommandStringTest::CommandStringTest(string _str) : Command(), str(_str) {}

CommandStringTest::~CommandStringTest() {}

void CommandStringTest::execute()
{
    cout << str << endl;
}