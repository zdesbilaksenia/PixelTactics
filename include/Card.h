#pragma once

#include "Clickable.h"
#include "Command.h"
#include "Unit.h"

class Card : public Clickable
{
public:

    Card(RenderWindow& _window, Mouse& _mouse, Command* _command);
    void setUnit(Unit* _unit);
    void draw() override;

    ~Card();

private:

    Unit* unit;
    //Label healthLabel;
    //Label attackLabel;
};