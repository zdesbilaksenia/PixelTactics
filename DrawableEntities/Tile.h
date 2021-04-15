#include <DrawableBox.h>
#include <Unit.h>

class Tile : public DrawableBox
{
public:
    Tile();
    Tile(int _side, int _mapX, int _mapY);
    void setUnit(Unit &_unit);
    void setStatus(int _status);
    int getStatus();
    ~Tile();

private:
    Unit &unit;
    int side;
    int status;
    int mapX;
    int mapY;
};