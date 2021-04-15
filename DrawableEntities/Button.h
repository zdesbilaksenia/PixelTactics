#include "Widget.h"
#include "../Commands/Command.h"

class Button : public Widget
{
public:
    Button();
    Button(string _text, Comand _cmd);
    void onClick();
    ~Button();

private:
    Comand comand;
};
