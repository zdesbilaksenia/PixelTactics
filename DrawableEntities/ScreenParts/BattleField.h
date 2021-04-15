#include "../Widget.h"
#include <vector>
#include <../Tile.h>

class BattleField
{
public:
    BattleField();
    BattleField(Image bckgr, int _width, int _height);
    void update();
    void draw();
    ~BattleField();

private:
    Image background;
    int width;
    int height;
    std::vector<Widget> widgets;
    std::vector<Tile> tiles;
};