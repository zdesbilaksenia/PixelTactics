#include "Widget.h"

class Label : public Widget
{
public:
    Label();
    Label(std::string _text);
    ~Label();
};