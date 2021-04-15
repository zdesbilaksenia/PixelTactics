#include "../Widget.h"
#include <vector>
#include <../Card.h>

class BottomPanel
{
public:
    BottomPanel();
    BottomPanel(Image bckgr, int _width, int _height);
    int getNumberOfCards();
    void update();
    void draw();
    ~BottomPanel();

private:
    Image background;
    int width;
    int height;
    std::vector<Widget> widgets;
    std::vector<Card> tiles;
};