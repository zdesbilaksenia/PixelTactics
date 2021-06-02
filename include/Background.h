#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

class Background
{
public:
    Background(RenderWindow &_window, Mouse &_mouse);
    void setTextures(Texture *_txBgr, Texture *_txLP, Texture *_txInf);
    void updateFocus();
    void draw();
    ~Background();

private:
    RenderWindow &window;
    Mouse &mouse;
    RectangleShape backgroundRect;
    RectangleShape lowerPanelRect;
    RectangleShape infoRect;
};