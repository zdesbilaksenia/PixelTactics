#pragma once
#include "DrawableBox.h"

enum Round
{
    start,
    avangard,
    flank,
    rear,
    opponentsTurn,
    won,
    lost
};

class StageText : public DrawableBox
{
public:
    StageText(RenderWindow &_window);
    void draw() override;
    void endDraw();
    void restart();
    void setRound(const Round &round);
    void setTextures(Texture* txGamestart, Texture *txAvangard, Texture *txFlank, Texture *txRear, Texture *txOpponentsTurn, Texture *txWon, Texture *txLost);

private:
    Texture textures[7];
    int moments;
};