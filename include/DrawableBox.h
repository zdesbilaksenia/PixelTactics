#pragma once

#include <SFML/Graphics.hpp>
#include <string>
using namespace sf;
using std::string;

class DrawableBox
{
public:
    DrawableBox(RenderWindow &_window);
    void setFillColor(const Color& _color);
    void setTexture(Texture *_tx);
    virtual void setSize(const int &_width, const int &_height);
    virtual void setPosition(const int &_posX, const int &_posY);

    const int &getPosX();
    const int &getPosY();
    const int &getWidth();
    const int &getHeight();

    void rotateTexture();

    virtual void setFont(const string& fontPath);

    virtual void draw() = 0;
    ~DrawableBox();

protected:
    int posX;
    int posY;
    int width;
    int height;
    RenderWindow &window;
    RectangleShape rect;
    Font font;
};