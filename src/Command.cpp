#include "../include/Command.h"
#include <iostream>
using namespace std;

Command::Command() {}

Command::~Command() {}

// ====================================
// ============CommandTest=============
// ====================================
CommandTest::CommandTest() {}

CommandTest::~CommandTest() {}

void CommandTest::execute()
{
    cout << "HEY" << endl;
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