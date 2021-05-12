#include "Command.h"
#include "Tile.h"

class CommandAttackTile : public Command
{
    public:
        CommandAttackTile(vector<Tile*>& _tiles);
        void execute() override;
        ~CommandAttackTile();

    private:
        //Нам же не нужная тут ссылка?
        //Поскольку важны объекты, а не массив
        vector<Tile*> tiles;
};
