#include "Clickable.h"
#include <iostream>
using namespace std;

Clickable::Clickable(RenderWindow &_window, Mouse &_mouse) : DrawableBox(_window),
                                                             mouse(_mouse), enabled(true)
{
}

Clickable::Clickable(RenderWindow &_window, Mouse &_mouse, Command *_command) : DrawableBox(_window),
                                                                                mouse(_mouse),
                                                                                command(_command),
                                                                                enabled(true)
{
}

Clickable::~Clickable()
{
    command = nullptr;
}

void Clickable::setCommand(Command *_command)
{
    this->command = _command;
}

bool Clickable::hasFocus()
{
    return rect.getGlobalBounds().contains(mouse.getPosition(window).x, mouse.getPosition(window).y);
}

void Clickable::updateFocus()
{
    if(enabled == false)
    {
        return;
    }
    isInFocus = this->hasFocus();
    if (isInFocus)
    {
        this->setFillColor(this->focusedColor);
    }
    else
    {
        this->setFillColor(this->basicColor);
    }
}

void Clickable::click()
{
    this->setFillColor(this->clickedColor);
    isPressed = true;
}

void Clickable::release()
{
    this->setFillColor(this->basicColor);
    isPressed = false;
}

//Потом удалить!!!

void Clickable::setColors(Color _basicColor, Color _focusedColor, Color _clickedColor, Color _disabledColor)
{
    basicColor = _basicColor;
    focusedColor = _focusedColor;
    clickedColor = _clickedColor;
    disabledColor = _disabledColor;
    this->setFillColor(this->basicColor);
}

void Clickable::enable()
{
    enabled = true;
    this->setFillColor(basicColor);
}

void Clickable::disable()
{
    enabled = false;
    this->setFillColor(disabledColor);
}

bool Clickable::isEnabled()
{
    return enabled;
}