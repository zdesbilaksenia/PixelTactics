#include "Widget.h"
#include "../Commands/Command.h"

class Button : public Widget
{
public:
    Button();
    Button(std::string _text, Command* _cmd);
    void onClick();
    ~Button();

private:
    Command* command;
};
