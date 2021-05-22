#pragma once

#include <SFML/Graphics.hpp>
#include "DrawableBox.h"
#include "Command.h"
//#include "../Commands/Command.h"
using namespace sf;

class Clickable : public DrawableBox
{
public:
    Clickable(RenderWindow &_window, Mouse &_mouse);
    Clickable(RenderWindow &_window, Mouse &_mouse, Command *_command);
    void setCommand(Command *_command);

    bool hasFocus();
    virtual void updateFocus();
    //Не у всех наследников есть этот метод
    virtual void click();
    void release();

    virtual void draw() = 0;
    ~Clickable();

    //Потом удалить!!
    void setColors(Color _basicColor, Color _focusedColor, Color _clickedColor, Color _disabledColor = Color::White);

    //Потом доделать
    void disable();
    void enable();
    bool isEnabled();

protected:
    const Mouse &mouse;
    //bool wasClicked;
    Command *command;

    bool isInFocus;
    bool isPressed;

    //Потом заменить на текстуры!
    Color basicColor;
    Color focusedColor;
    Color clickedColor;
    Color disabledColor;

//Это потом применить надо (типа чтобы кнопка не нажималась)
    bool enabled;
    //const Color disabledColor = Color(40, 40, 40);
};
