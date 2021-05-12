#include "../Commands/Command.h"
#include <vector>

class CommandManager
{
    //Reciever reciever
public:
    CommandManager();
    void PushCommand(Command);
    void sendCommands();

private:
    std::vector<Command> commands;
}