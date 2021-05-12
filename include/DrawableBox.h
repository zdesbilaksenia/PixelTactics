#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class DrawableBox
{
public:
    DrawableBox(RenderWindow& _window);
    void setFillColor(Color _color);
    void setTexture(Texture* _tx);
    void setSize(int _width, int _height);
    void setPosition(int _posX, int _posY);
    virtual void draw() = 0;
    ~DrawableBox();

//Пока сделал protected
protected:
    RenderWindow& window;
    RectangleShape rect;
};