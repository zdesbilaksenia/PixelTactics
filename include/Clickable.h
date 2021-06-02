#pragma once

#include <SFML/Graphics.hpp>
#include "DrawableBox.h"
#include "Command.h"
using namespace sf;

class Clickable : public DrawableBox
{
public:
    Clickable(RenderWindow &_window, Mouse &_mouse);
    Clickable(RenderWindow &_window, Mouse &_mouse, Command& _command);

    bool hasFocus();
    virtual void updateFocus();
    //Не у всех наследников есть этот метод
    virtual void click();
    void release();

    virtual void draw() = 0;
    ~Clickable();

    void setColors(Color _basicColor, Color _focusedColor, Color _clickedColor, Color _disabledColor = Color::White);

    void disable();
    void enable();
    bool isEnabled();

protected:
    const Mouse &mouse;
    Command *command;

    bool isInFocus;
    bool isPressed;

    //Потом заменить на текстуры!
    Color basicColor;
    Color focusedColor;
    Color clickedColor;
    Color disabledColor;

    bool enabled;
};
