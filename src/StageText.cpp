#include "StageText.h"
#include "configurations.cpp"

StageText::StageText(RenderWindow &_window) : DrawableBox(_window)
{
    this->setSize(textWidth, textHeight);
    this->setPosition(textPosX, textPosY);
}

void StageText::draw()
{
    if (moments >= 0 && moments < maxNumberOfMoments)
    {
        moments++;
        window.draw(rect);
        return;
    }
    moments = -1;
}

void StageText::restart()
{
    moments = 0;
}

void StageText::setRound(const Round &round)
{
    this->setTexture(&textures[round]);
}

void StageText::setTextures(Texture* txGamestart, Texture *txAvangard, Texture *txFlank, Texture *txRear, Texture *txOpponentsTurn, Texture *txWon, Texture *txLost)
{
    textures[0] = *txGamestart;
    textures[1] = *txAvangard;
    textures[2] = *txFlank;
    textures[3] = *txRear;
    textures[4] = *txOpponentsTurn;
    textures[5] = *txWon;
    textures[6] = *txLost;
}
