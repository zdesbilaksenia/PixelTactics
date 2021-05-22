#include "../include/Button.h"
#include "configurations.cpp"

//Удалить
#include <iostream>
using namespace std;

Button::Button(RenderWindow &_window, Mouse &_mouse, Command *_command) : Clickable(_window, _mouse, _command)
{
    this->setSize(buttonWidth, buttonHeight);
}

void Button::click()
{
    Clickable::click();
    this->command->execute();
}

void Button::draw()
{
    window.draw(rect);
}

Button::~Button()
{
    command = nullptr;
}

//=====================================
//===========ButtonsManager============
//=====================================

ButtonsManager::ButtonsManager()
{
}

void ButtonsManager::setButtons(vector<Button *> _buttons)
{
    for (auto _button : _buttons)
    {
        buttons.push_back(_button);
    }
}

void ButtonsManager::updateFocus()
{
    // Смотрим, какие кнопки в фокусе, что бы их соответствующим образом отрисовывать
    for (auto button : buttons)
    {
        button->updateFocus();
    }
}

void ButtonsManager::mouseIsPressed()
{
    // Смотрим, на какую кнопку нажали
    for (auto button : buttons)
    {
        if (button->hasFocus())
        {
            button->click();
        }
    }
}

void ButtonsManager::mouseIsReleased()
{
    //Это что-бы цвет нажатой кнопки заменился на цвет фокуса
    for (auto button : buttons)
    {
        if (button->hasFocus())
        {
            button->updateFocus();
        }
    }
}

void ButtonsManager::draw()
{
    for (auto button : buttons)
    {
        button->draw();
    }
}

ButtonsManager::~ButtonsManager()
{
}