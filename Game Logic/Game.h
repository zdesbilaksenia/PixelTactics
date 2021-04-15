#include <iostream>
#include "Player.h"

class Game{
public:
    Game();
    
    void MakeMove();

private:
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

    void startGame();

    void shuffleCards();
    void decideFirst();
    void ChooseLeader();

    void endGame();
};