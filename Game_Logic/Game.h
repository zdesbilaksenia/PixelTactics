#include <iostream>
#include <vector>
#include "Player.h"
#include "Position.h"

/*Class IStartGame(){
    virtual void shuffleCards();
    virtual void decideFirst();
    virtual void ChooseLeader();
}*/

class Game{
public:

    Game(Game &other) = delete;
    void operator = (const Game& other) = delete;

    //Singleton Realisation
    static Game *GetInstance();
    
    //Сделать ход. Передать управление игроку
    void MakeMove();

    void GetLine();

    void GetMove();

    Player& GetFirstPlayer() {return firstPlayer};
    Player& GetSecondPlayer() {return secondPlayer};

    void GetPole();

protected:
    Game();
    static Game* game_;

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
    //Поле
    std::vector <Position> Pole;
    //Начало игры
    void startGame();

    //Методы начала игры
    //Раздача Карт, определение первого игрока и лидера 
    void shuffleCards();
    void decideFirst();
    void ChooseLeader();
    void InitializePole();

    //Конец игры
    void endGame();
};

Game* Game::game_ = nullptr;

Game* Game::GetInstance(){
    if(game_ = nullptr){
        game_ = new game();
    }
    
    return game_;
};