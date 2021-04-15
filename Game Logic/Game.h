#include <iostream>
#include "Player.h"

/*Class IStartGame(){
    virtual void shuffleCards();
    virtual void decideFirst();
    virtual void ChooseLeader();
}*/

class Game{
public:
    
    //Сделать ход. Передать управление игроку
    void MakeMove();

    void GetLine();

    void GetMove();

protected:
    Game();
    static Game* game_;

private:
    Game(Game &other) = delete;
    void operator = (const )
    //Ход
    unsigned int move;
    //Линия для размещения
    unsigned int currentLine;
    //Первый игрок
    Player firstPlayer;
    //Второй игрок
    Player secondPlayer;
    //Время на ход
    unsigned int timePerMove;
    //Очередь хода
    unsigned int turn;

    //Начало игры
    void startGame();

    //Методы начала игры
    //Раздача Карт, определение первого игрока и лидера 
    void shuffleCards();
    void decideFirst();
    void ChooseLeader();

    //Конец игры
    void endGame();
};