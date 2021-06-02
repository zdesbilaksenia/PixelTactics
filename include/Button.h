#pragma once

#include "Clickable.h"
#include <vector>
using std::vector;

#include <boost/log/trivial.hpp>
using namespace boost::log;

class Button : public Clickable
{
public:
    Button(RenderWindow &_window, Mouse &_mouse, Command& _command);
    void click() override;
    void draw() override;
    ~Button();

private:
};

//=====================================
//===========ButtonsManager============
//=====================================

class ButtonsManager
{
public:
    ButtonsManager();

    void setButtons(vector<Button *> &_buttons);
    void updateFocus();
    void mouseIsPressed();
    void mouseIsReleased();
    void draw();
    void disable();
    void enable();

    ~ButtonsManager();

private:
    //Потом заменить указатели на умные указатели
    vector<Button *> buttons;
};