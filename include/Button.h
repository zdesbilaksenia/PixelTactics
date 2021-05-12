#pragma once

#include "Clickable.h"

class Button : public Clickable
{
public:
    Button(RenderWindow& _window, Mouse& _mouse, Command* _command);
    void click() override;
    void draw() override;

    ~Button();

private:

};

class ButtonManager
{
    public:
    ButtonManager();

    
    void draw();

    ~ButtonManager();
};
