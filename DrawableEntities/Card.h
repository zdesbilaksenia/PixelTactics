#include "DrawableBox.h"
#include "Label.h"

class Card : public DrawableBox
{
public:
    Card();
    Card(int _characterId);
    int getCharacterId();
    ~Card();

private:
    int characterId;
    Label healthLabel;
    Label attackLabel;
};