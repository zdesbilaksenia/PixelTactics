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
    Background(RenderWindow &_window);
    void setTextures(Texture* _txBgr, Texture* _txLP);
    void draw();
    void setStage(StageBackground _stage);
    ~Background();

private:
    RenderWindow &window;
    RectangleShape backgroundRect;
    RectangleShape lowerPanelRect;
};