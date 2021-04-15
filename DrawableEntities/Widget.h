#include "DrawableBox.h"

class Widget : public DrawableBox
{
public:
    Widget();
    void setText(std::string _text);
    ~Widget();

private:
    std::string text;
};