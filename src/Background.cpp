#include "Background.h"
#include "configurations.cpp"

Background::Background(RenderWindow &_window) : window(_window)
{
    backgroundRect.setPosition(backgroundPosX, backgroundPosY);
    backgroundRect.setSize(Vector2f(backgroundWidth, backgroundHeight));
    lowerPanelRect.setPosition(lowerPanelPosX, lowerPanelPosY);
    lowerPanelRect.setSize(Vector2f(lowerPanelWidth, lowerPanelHeight));
}

void Background::setTextures(Texture *_txBgr, Texture *_txLP)
{
    backgroundRect.setTexture(_txBgr);
    lowerPanelRect.setTexture(_txLP);
}

void Background::draw()
{
    window.draw(backgroundRect);
    window.draw(lowerPanelRect);
}

Background::~Background() {}