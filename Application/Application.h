#include <DrawableEntities/ScreenParts/BattleField.h>
#include <DrawableEntities/ScreenParts/BottomPanel.h>
#include <CommandManager/CommandManager.h>

class Application
{
public:
    Application();
    Application(BattleField &_battleField, BottomPanel &_bottomPanel, CommandManager &_cmdManager);
    void start();
    ~Application();

private:
    BattleField battleField;
    BottomPanel bottomPanel;
    CommandManager cmdManager;
}