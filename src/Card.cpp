#include "Card.h"

Card::Card(RenderWindow &_window, Mouse &_mouse, Command *_command) : Clickable(_window, _mouse, _command) {}

void Card::setUnit(Unit *_unit)
{
    this->unit = _unit;
}

void Card::draw()
{
    window.draw(rect);

    //Цифры взяты с потолка!
    //Скорее всего, потом надо будет сделать CardManager
    unit->setPosition(rect.getPosition().x + 10, rect.getPosition().y + 10);
    
    unit->draw();
}

Card::~Card()
{
    unit = nullptr;
}
