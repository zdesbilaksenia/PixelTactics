#include "../include/DrawableBox.h"

DrawableBox::DrawableBox(RenderWindow& _window) : window(_window) 
{
    rect.setSize(Vector2f(100, 100));
    rect.setPosition(0, 0);
    rect.setFillColor(Color::White);
};

void DrawableBox::setFillColor(Color _color) 
{
    rect.setFillColor(_color);
}

void DrawableBox::setTexture(Texture* _tx) 
{
    rect.setTexture(_tx);
}

void DrawableBox::setSize(int _width, int _height)
{
    rect.setSize(Vector2f(_width, _height));
}

void DrawableBox::setPosition(int _posX, int _posY)
{
    rect.setPosition(_posX, _posY);
}

DrawableBox::~DrawableBox() {}