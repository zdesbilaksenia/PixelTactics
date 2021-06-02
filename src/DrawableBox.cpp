#include "DrawableBox.h"

#include <iostream>
using namespace std;

DrawableBox::DrawableBox(RenderWindow &_window) : window(_window)
{
    this->width = 100;
    this->height = 100;
    this->posX = 0;
    this->posY = 0;
    rect.setSize(Vector2f(100, 100));
    rect.setPosition(0, 0);
    rect.setFillColor(Color::White);
}

void DrawableBox::setFillColor(const Color &_color)
{
    rect.setFillColor(_color);
}

void DrawableBox::setTexture(Texture *_tx)
{
    rect.setTexture(_tx);
}

void DrawableBox::setSize(const int &_width, const int &_height)
{
    this->width = _width;
    this->height = _height;
    rect.setSize(Vector2f(_width, _height));
}

void DrawableBox::setPosition(const int &_posX, const int &_posY)
{
    this->posX = _posX;
    this->posY = _posY;
    rect.setPosition(_posX, _posY);
}

const int &DrawableBox::getPosX()
{
    return posX;
}

const int &DrawableBox::getPosY()
{
    return posY;
}

const int &DrawableBox::getWidth()
{
    return width;
}

const int &DrawableBox::getHeight()
{
    return height;
}

void DrawableBox::rotateTexture()
{
    rect.setOrigin(Vector2f(rect.getLocalBounds().width, 0));
    rect.scale(Vector2f(-1, 1));
    //cout << "posX = " << posX << " ";
    //posX += width;
    //cout << "posX = " << posX << endl;
    //this->setPosition(posX, posY);
}

void DrawableBox::setFont(const string &fontPath)
{
    font.loadFromFile(fontPath);
}

DrawableBox::~DrawableBox() {}