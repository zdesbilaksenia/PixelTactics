#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class DrawableBox
{
public:
    DrawableBox(RenderWindow& _window);
    void setFillColor(Color _color);
    void setTexture(Texture* _tx);
    virtual void setSize(const int& _width, const int& _height);
    virtual void setPosition(const int& _posX, const int& _posY);

    const int& getPosX();
    const int& getPosY();
    const int& getWidth();
    const int& getHeight();

    virtual void draw() = 0;
    ~DrawableBox();

//Пока сделал protected
protected:
    int posX;
    int posY;
    int width;
    int height;
    RenderWindow& window;
    RectangleShape rect;
};