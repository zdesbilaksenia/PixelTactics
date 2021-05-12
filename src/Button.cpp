#include "../include/Button.h"

//Удалить
#include <iostream>
using namespace std;

Button::Button(RenderWindow& _window, Mouse& _mouse, Command* _command) : Clickable(_window, _mouse, _command){}

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