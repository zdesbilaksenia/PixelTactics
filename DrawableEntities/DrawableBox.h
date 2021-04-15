#include <SFML/Graphics.hpp>
using namespace sf;

class DrawableBox
{
public:
    DrawableBox();
    void setTexture(Texture _tx);
    void setSize(int _width, int _height);
    void setPosition(int _posX, int _posY);
    void setPosX(int _posX);
    void setPosY(int _posY);
    int getPosX();
    int getPosY();
    void draw();
    ~DrawableBox();

private:
    RectangleShape rect;
    int width;
    int height;
    int posX;
    int posX;
};