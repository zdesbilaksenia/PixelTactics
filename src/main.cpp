#include "player.h"

int main(){
    Pole poleshko;

    /*for(int i = 0; i < pole_.size(); i++){
       std::cout << pole_[i].GetCell() << pole_[i].GetLine() << pole_[i].GetSide() << std::boolalpha << pole_[i].isEmpty() << std::endl;
    }

    std::cout << "//////////////////////////" << std::endl;*/

    
    Position FirstPlayerLeader(1,1,0);
    Position SecondPlayerLeader(1,1,1);

    std::cout << "//////////////////////////" << std::endl;
    
    /*std::cout << test.GetCell() << test.GetLine() << test.GetSide() << std::boolalpha << test.isEmpty() << std::endl;

    std::cout << "//////////////////////////" << std::endl;*/
    std::vector<Card*> LeaderCards; 
    int choice = 0;

    Breed LeaderMageBreed(NULL,15,7);
    Card* LeaderMageCard = LeaderMageBreed.NewCard();
    LeaderCards.push_back(LeaderMageCard);

    Breed LeaderWarriorBreed(NULL,22,6);
    Card* LeaderWarriorCard = LeaderWarriorBreed.NewCard();
    LeaderCards.push_back(LeaderWarriorCard);

    Breed LeaderPriestBreed(NULL,30,3);
    Card* LeaderPriestCard = LeaderPriestBreed.NewCard();
    LeaderCards.push_back(LeaderPriestCard);

    std::cout << "Выбери лидера Первый игрок" << std::endl;
    for(int i = 0; i < LeaderCards.size(); i++){
        std::cout << "Здоровье Героя:"<< LeaderCards[i]->GetBreed().GetHealth() << " " << "Атака Героя:" << LeaderCards[i]->GetBreed().GetStrength() << std::endl;
    }
    std::cin >> choice;
    FirstPlayerLeader.SetHero(LeaderCards[choice]);
    poleshko.SetPosition(FirstPlayerLeader);

    std::cout << "Выбери лидера Второй игрок" << std::endl;
    for(int i = 0; i < LeaderCards.size(); i++){
        std::cout << "Здоровье Героя:"<< LeaderCards[i]->GetBreed().GetHealth() << " " << "Атака Героя:" << LeaderCards[i]->GetBreed().GetStrength() << std::endl;
    }
    std::cin >> choice;
    SecondPlayerLeader.SetHero(LeaderCards[choice]);
    poleshko.SetPosition(SecondPlayerLeader);
    

    std::vector<Card*> useCard;

    Breed MageBreed(NULL,10,5);
    Card* MageCard = MageBreed.NewCard();
    useCard.push_back(MageCard);

    Breed WarriorBreed(NULL,16,4);
    Card* WarriorCard = WarriorBreed.NewCard();
    useCard.push_back(WarriorCard);


    Breed PriestBreed(NULL,20,2);
    Card* PriestCard = PriestBreed.NewCard();
    useCard.push_back(PriestCard);


    Deck FirstDeck(useCard);
    Deck SecondDeck(useCard);

    Player FirstPlayer(FirstDeck,0);
    /*FirstPlayer.GetDeck().ShowDeck();

    std::cout << "//////////////////////////" << std::endl;*/

    Player SecondPlayer(SecondDeck,1);
    /*SecondPlayer.GetDeck().ShowDeck();

    std::cout << "//////////////////////////" << std::endl;*/
    FirstPlayer.StartingHand();
    SecondPlayer.StartingHand();

    int currentside = 0;
    Player CurrentPlayer = FirstPlayer;
    int MovesAmount = 2;

    while(poleshko.CheckLeader()){
        if(MovesAmount == 0){
            if(currentside == 0){
                std::cout << "Ход переходит ко второму игроку" << std::endl;
                currentside = 1;
                CurrentPlayer = SecondPlayer;
            }else{
                std::cout << "Ход переходит ко первому игроку" << std::endl;
                currentside = 0;
                CurrentPlayer = FirstPlayer;
            }
            MovesAmount = 2;
        }
        if(currentside == 0) 
            std::cout << "Ход Первого игрока" <<std::endl;
        else
            std::cout << "Ход Второго игрока" <<std::endl;

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
                Position kletka = poleshko.GetPosition(cell,line,side);
                kletka.InfoPosition();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }
            case(4):{
                std::cout << "Введите клетку и линию вашего героя"  << std::endl;
                int line = 0;
                int cell = 0;

                std::cin >> cell >> line;
                Position YourHero = poleshko.GetPosition(cell,line,CurrentPlayer.GetSide());

                std::cout << "Введите клетку и линию вражеского героя"  << std::endl;
                std::cin >> cell >> line;
                int EnemySide;
                if(CurrentPlayer.GetSide() == 0){
                    EnemySide = 1;
                }else{
                    EnemySide = 0;
                }
                Position EnemyHero = poleshko.GetPosition(cell,line,EnemySide);

                if(CurrentPlayer.MeleeAttackCheck(EnemyHero,poleshko)){
                CurrentPlayer.Attack(YourHero,EnemyHero);
                poleshko.SetPosition(EnemyHero);
                MovesAmount--;
                } else {
                    std::cout << "СПЕРЕДИ СТОИТ ДРУГОЙ ГЕРОЙ. НЕЛЬЗЯ АТАКОВАТЬ!!"  << std::endl;
                }
                break;
            }
            case(5):{
                poleshko.Show();
                break;
            }
            case(6):{
                std::cout << "Выберете карту" <<std::endl;
                std::cin >> choice;
                Card* ChosenCard = CurrentPlayer.ChooseCard(choice); 

                std::cout << "Введите клетку и линию"  << std::endl;
                int line = 0;
                int cell = 0;
                std::cin >> cell >> line;

                Position kletka = poleshko.GetPosition(cell,line,CurrentPlayer.GetSide());
                kletka.SetHero(ChosenCard);
                poleshko.SetPosition(kletka);
                MovesAmount --;
                break;
            }
            default:{
                break;
            }
        }
    }

    /*Card* FirstLeaderCard;
    FirstLeaderCard = FirstPlayer.ChooseCard(0);
    Position FirstPlayerLeader(1,1,0);
    FirstPlayerLeader.SetHero(FirstLeaderCard);

    Card* SecondLeaderCard;
    SecondLeaderCard = SecondPlayer.ChooseCard(0);
    Position SecondPlayerLeader(1,1,1);
    SecondPlayerLeader.SetHero(SecondLeaderCard);
    //std::cout << test.GetCell() << test.GetLine() << test.GetSide() << std::boolalpha << test.isEmpty() << std::endl;

    std::cout << "//////////////////////////" << std::endl;
    SetPosition(pole_,FirstPlayerLeader);
    SetPosition(pole_,SecondPlayerLeader);
    Show(pole_);
    std::cout << "//////////////////////////" << std::endl;

    SecondPlayer.GetHand().ShowDeck();

    std::cout << "//////////////////////////" << std::endl;

    infoPosition(pole_,FirstPlayerLeader);
    infoPosition(pole_,SecondPlayerLeader);
*/
    /*std::cout << "//////////////////////////" << std::endl;
    while(CheckLeader(FirstPlayerLeader,SecondPlayerLeader,pole_)){
        Attack(SecondPlayer,SecondPlayerLeader,FirstPlayerLeader,pole_);
        infoPosition(pole_,FirstPlayerLeader);
    }*/
    


    return 0;
}