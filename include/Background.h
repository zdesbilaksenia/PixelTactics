#pragma once

#include <SFML/Graphics.hpp>
using namespace sf;

enum class StageBackground
{
    //Ещё добавить "other player's turn"
    stageAvangard,
    stageFlank,
    stageRear
};

class Background
{
public:
    Background(RenderWindow &_window, Mouse &_mouse);
    void setTextures(Texture *_txBgr, Texture *_txLP);
    void setStage(StageBackground _stage);
    void updateFocus();
    void draw();
    ~Background();

private:
    RenderWindow &window;
    Mouse &mouse;
    RectangleShape backgroundRect;
    RectangleShape lowerPanelRect;
};