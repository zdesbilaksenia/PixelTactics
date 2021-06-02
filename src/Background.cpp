#include "Background.h"
#include "configurations.cpp"

Background::Background(RenderWindow &_window, Mouse &_mouse) : window(_window), mouse(_mouse)
{
    backgroundRect.setPosition(backgroundPosX, backgroundPosY);
    backgroundRect.setSize(Vector2f(backgroundWidth, backgroundHeight));
    lowerPanelRect.setPosition(lowerPanelPosX, lowerPanelPosY);
    lowerPanelRect.setSize(Vector2f(lowerPanelWidth, lowerPanelHeight));
    infoRect.setSize(Vector2f(infoRectWidth, infoRectHeight));
    infoRect.setPosition(Vector2f(infoRectPosX, infoRectPosY));
}

void Background::setTextures(Texture *_txBgr, Texture *_txLP, Texture *_txInf)
{
    if (_txBgr == nullptr || _txLP == nullptr || _txInf == nullptr)
    {
        return;
    }
    backgroundRect.setTexture(_txBgr);
    lowerPanelRect.setTexture(_txLP);
    infoRect.setTexture(_txInf);
}

void Background::updateFocus()
{
    backgroundRect.setPosition(Vector2f(mouse.getPosition().x * parallaxCoefficient, backgroundPosY));
}

void Background::draw()
{
    window.draw(backgroundRect);
    window.draw(lowerPanelRect);
    window.draw(infoRect);
}

Background::~Background() {}