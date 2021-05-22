#include <iostream>
#include <vector>
#include "../lib/pole.h"
#include "../lib/player.h"
#include "../DataBase/MySQL.h"
#include "request.h"


int main(){
    Pole pole;

    Position FirstPlayerLeader(1,1,0);
    Position SecondPlayerLeader(1,1,1);

    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());

    std::cout << "//////////////////////////" << std::endl;
    Deck FirstLeaderCards;
    Deck SecondLeaderCards;


    for (size_t i = 0; i < 3; i++){
        int random = allCards.roll_card(allCards.GetVector().size());
        FirstLeaderCards.push_back(allCards.GetVector()[random]);
    }

    for (size_t i = 0; i < 3; i++){
        int random = allCards.roll_card(allCards.GetVector().size());
        SecondLeaderCards.push_back(allCards.GetVector()[random]);
    }

    std::cout << "Выбери лидера Первый игрок" << std::endl;
    for(int i = 0; i < FirstLeaderCards.GetVector().size(); i++){
        std::cout << FirstLeaderCards.GetVector()[i].name << " "<< "Здоровье Героя:"<< FirstLeaderCards.GetVector()[i].HP << " " << "Атака Героя:" << FirstLeaderCards.GetVector()[i].strength << std::endl;
    }
    int choice;
    std::cin >> choice;
    FirstPlayerLeader.SetHero(FirstLeaderCards.GetVector()[choice]);
    FirstPlayerLeader.GetHero().MakeLeader();
    pole.SetPosition(&(FirstPlayerLeader));

    std::cout << "Выбери лидера Второй игрок" << std::endl;
    for(int i = 0; i < SecondLeaderCards.GetVector().size(); i++){
        std::cout <<SecondLeaderCards.GetVector()[i].name << " "<< "Здоровье Героя:"<< SecondLeaderCards.GetVector()[i].HP << " " << "Атака Героя:" << SecondLeaderCards.GetVector()[i].strength << std::endl;
    }
    std::cin >> choice;
    SecondPlayerLeader.SetHero(SecondLeaderCards.GetVector()[choice]);
    SecondPlayerLeader.GetHero().MakeLeader();
    pole.SetPosition(&(SecondPlayerLeader));

    //pole.Show();
    std::cout << "//////////////////////////" << std::endl;

    Player FirstPlayer(FirstPlayerDeck,0);
    Player SecondPlayer(SecondPlayerDeck,1);

    FirstPlayer.StartingHand();
    SecondPlayer.StartingHand();


    int currentside = 0;
    Player CurrentPlayer = FirstPlayer;
    int MovesAmount = 2;
    int Phase = 0;

    while(pole.CheckLeader()){
        if(MovesAmount == 0){
            if(currentside == 0){
                std::cout << "Ход переходит ко второму игроку" << std::endl;
                currentside = 1;
                CurrentPlayer = SecondPlayer;
            }else{
                std::cout << "Ход переходит ко первому игроку" << std::endl;
                currentside = 0;
                CurrentPlayer = FirstPlayer;
                Phase++;
            }
            if(Phase == 3){
                Phase = 0;
            }
            MovesAmount = 2;
        }
        if(currentside == 0) 
            std::cout << "Ход Первого игрока" <<std::endl;
        else
            std::cout << "Ход Второго игрока" <<std::endl;
        switch(Phase){
            case(0):{
                std::cout << "Стадия Авангарда" <<std::endl;
                break;
            }
            case(1):{
                std::cout <<"Стадия Фланга"<<std::endl;
                break;
            }
            case(2):{
                std::cout <<"Стадия тыла" <<std::endl;
                break;
            }
        }

        std::cout << "//////////////////////////" << std::endl;
        std::cout <<"Взять карту(1)" << std::endl;
        std::cout <<"Посмотреть руку(2)" << std::endl;
        std::cout <<"Получить информацию о герое на поле(3)" << std::endl;
        std::cout <<"Атаковать своим героем врага(4)" << std::endl;
        std::cout <<"Посмотреть поле(5)" << std::endl;
        std::cout <<"Поставить героя(6)" << std::endl;
        std::cout << "//////////////////////////" << std::endl;

        choice = 0;
        std::cin >> choice;
        switch(choice){
            case(1):{
                CurrentPlayer.DrawCard();
                MovesAmount --;
                break;
            }
            case(2):{
                CurrentPlayer.GetHand().ShowDeck();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }
            case(3):{
                std::cout << "Введите клетку и линию и сторону"  << std::endl;
                int line = 0;
                int cell = 0;
                int side = 0;

                std::cin >> cell >> line >> side;
                Position* kletka = pole.GetPosition(cell,line,side);
                kletka->InfoPosition();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }
            case(4):{
                std::cout << "Введите клетку и линию вашего героя"  << std::endl;
                int line = 0;
                int cell = 0;

                std::cin >> cell >> line;
                Position* YourHero = pole.GetPosition(cell,line,CurrentPlayer.GetSide());

                std::cout << "Введите клетку и линию вражеского героя"  << std::endl;
                std::cin >> cell >> line;
                int EnemySide;
                if(CurrentPlayer.GetSide() == 0){
                    EnemySide = 1;
                }else{
                    EnemySide = 0;
                }
                Position* EnemyHero = pole.GetPosition(cell,line,EnemySide);

                if(CurrentPlayer.MeleeAttackCheck(EnemyHero,pole)){
                //CurrentPlayer.Attack(YourHero,EnemyHero);
                YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());
                pole.SetPosition(EnemyHero);
                MovesAmount--;
                } else {
                    std::cout << "СПЕРЕДИ СТОИТ ДРУГОЙ ГЕРОЙ. НЕЛЬЗЯ АТАКОВАТЬ!!"  << std::endl;
                }
                break;
            }
            case(5):{
                pole.Show();
                std::vector<bool> Test;
                Test = pole.CanBeMeleeAttackedRequest(CurrentPlayer.GetSide());
                for(int i = 0; i < Test.size(); i++){
                    std::cout << std::boolalpha << Test[i] << " ";
                }
                break;
            }
            case(6):{
                std::cout << "Выберете карту" <<std::endl;
                std::cin >> choice;
                Card ChosenCard = CurrentPlayer.ChooseCard(choice); 

                std::cout << "Введите клетку и линию"  << std::endl;
                int line = 0;
                int cell = 0;
                std::cin >> cell >> line;

                Position* kletka = pole.GetPosition(cell,line,CurrentPlayer.GetSide());
                kletka->SetHero(ChosenCard);
                pole.SetPosition(kletka);
                MovesAmount --;
                break;
            }
            case(7):{

            }
            default:{
                break;
            }
        }
    }

    return 0;
}