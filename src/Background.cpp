#include "Background.h"
#include "configurations.cpp"

Background::Background(RenderWindow &_window, Mouse &_mouse) : window(_window), mouse(_mouse)
{
    backgroundRect.setPosition(backgroundPosX , backgroundPosY);
    backgroundRect.setSize(Vector2f(backgroundWidth, backgroundHeight));
    lowerPanelRect.setPosition(lowerPanelPosX, lowerPanelPosY);
    lowerPanelRect.setSize(Vector2f(lowerPanelWidth, lowerPanelHeight));
}

void Background::setTextures(Texture *_txBgr, Texture *_txLP)
{
    if (_txBgr == nullptr || _txLP == nullptr)
    {
        return;
    }
    backgroundRect.setTexture(_txBgr);
    lowerPanelRect.setTexture(_txLP);
}

void Background::updateFocus()
{
    backgroundRect.setPosition(Vector2f(mouse.getPosition().x * parallaxCoefficient, backgroundPosY));
}

void Background::draw()
{
    window.draw(backgroundRect);
    window.draw(lowerPanelRect);
}

Background::~Background() {}