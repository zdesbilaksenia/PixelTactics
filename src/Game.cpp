#include "Game.h"
#include "request.h"

void Game::StartGame(){
    Pole pole;

    Position FirstPlayerLeader(1, 1, 0);
    Position SecondPlayerLeader(1, 1, 1);

    MySQL sql;
    Deck allCards;
    allCards = sql.GetCards();
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player FirstPlayer(FirstPlayerDeck, 0);
    Player SecondPlayer(SecondPlayerDeck, 1);
    std::vector<Card> StepaFirstAllCards = FirstPlayerDeck.GetVector();
    auto msg = Message<GameMsgTypes>(GameMsgTypes::GameDeck);
    msg << StepaFirstAllCards;
    CallPechkin(0,msg);

    std::vector<Card> StepaSecondAllCards = FirstPlayerDeck.GetVector();
    msg = Message<GameMsgTypes>(GameMsgTypes::GameDeck);
    msg << StepaSecondAllCards;
    CallPechkin(1,msg);

    std::cout << "//////////////////////////" << std::endl;
    Deck FirstLeaderCards;
    Deck SecondLeaderCards;

    PowerMapper mapper;

    std::cout << "Выбери лидера Первый игрок" << std::endl;
    for (int i = 17; i < FirstPlayerDeck.GetVector().size(); i++) {
        std::cout << FirstPlayerDeck.GetVector()[i].name << " " << "Здоровье Героя:"
                  << FirstPlayerDeck.GetVector()[i].HP << " " << "Атака Героя:"
                  << FirstPlayerDeck.GetVector()[i].strength << std::endl;
    }
    int choice;
    std::cout << "////" << std::endl;
    lobby->incoming().wait();
    auto leadmsg = lobby->incoming().popFront().msg;
    if (leadmsg.header.id == GameMsgTypes::GameCardFromHandChoice) {
       leadmsg >> choice;
    } else {
        std::cout << "Error in leader choice!\n";
        return;
    }
    std::cout << choice << std::endl;
    for (int i = 0; i < FirstPlayerDeck.GetVector().size(); i++) {
        std::cout << FirstPlayerDeck.GetVector()[i].name << " " << "Здоровье Героя:"
                  << FirstPlayerDeck.GetVector()[i].HP << " " << "Атака Героя:"
                  << FirstPlayerDeck.GetVector()[i].strength << std::endl;
    }
    FirstPlayerLeader.SetHero(FirstPlayerDeck.GetVector()[choice]);
    mapper.MapPowers(FirstPlayerLeader.GetHero());
    FirstPlayerLeader.GetHero().MakeLeader();
    pole.SetPosition(&(FirstPlayerLeader));
    FirstPlayerDeck.GetVector().erase(FirstPlayerDeck.GetVector().begin() + choice);
    FirstPlayer.GetHand().push_back(FirstPlayerDeck.GetVector().back());
    FirstPlayerDeck.GetVector().pop_back();
    FirstPlayer.GetHand().push_back(FirstPlayerDeck.GetVector().back());
    FirstPlayerDeck.GetVector().pop_back();

    FirstPlayerLeader.InfoPosition();
    for(int i = 0; i < FirstPlayer.GetHand().GetVector().size(); i++){
        std::cout << FirstPlayer.GetHand().GetVector()[i].name << " " << "Здоровье Героя:"
                  << FirstPlayer.GetHand().GetVector()[i].HP << " " << "Атака Героя:"
                  << FirstPlayer.GetHand().GetVector()[i].strength << std::endl;
    }

    std::cout << "Выбери лидера Второй игрок" << std::endl;
    for (int i = 17; i < SecondPlayerDeck.GetVector().size(); i++) {
        std::cout << SecondPlayerDeck.GetVector()[i].name << " " << "Здоровье Героя:"
                  << SecondPlayerDeck.GetVector()[i].HP << " " << "Атака Героя:"
                  << SecondPlayerDeck.GetVector()[i].strength << std::endl;
    }

    FirstPlayerLeader.InfoPosition();
    for(int i = 0; i < SecondPlayer.GetHand().GetVector().size(); i++){
        std::cout << SecondPlayer.GetHand().GetVector()[i].name << " " << "Здоровье Героя:"
                  << SecondPlayer.GetHand().GetVector()[i].HP << " " << "Атака Героя:"
                  << SecondPlayer.GetHand().GetVector()[i].strength << std::endl;
    }
    lobby->incoming().wait();
    leadmsg = lobby->incoming().popFront().msg;
    if (leadmsg.header.id == GameMsgTypes::GameCardFromHandChoice) {
        leadmsg >> choice;
    } else {
        std::cout << "Error in leader choice!\n";
        return;
    }
    SecondPlayerLeader.SetHero(SecondPlayerDeck.GetVector()[choice]);
    mapper.MapPowers(SecondPlayerLeader.GetHero());
    SecondPlayerLeader.GetHero().MakeLeader();
    pole.SetPosition(&(SecondPlayerLeader));
    SecondPlayerDeck.GetVector().erase(SecondPlayerDeck.GetVector().begin() + choice);
    SecondPlayer.GetHand().push_back(SecondPlayerDeck.GetVector().back());
    SecondPlayerDeck.GetVector().pop_back();
    SecondPlayer.GetHand().push_back(SecondPlayerDeck.GetVector().back());
    SecondPlayerDeck.GetVector().pop_back();


    // Breed allStats[pole.GetVector().size()];
    std::vector<Breed> allStats;
    allStats.resize(0);
    for (int i = 0; i < pole.GetVector().size(); i++) {
        Breed stats;
        stats.HP = pole.GetVector()[i]->GetHero().GetCurHealth();
        stats.Strength = pole.GetVector()[i]->GetHero().GetCurStrength();
        allStats.push_back(stats);
    }
    Message<GameMsgTypes> Inmsg(GameMsgTypes::HeroesStats);
    msg << allStats;
    CallPechkin(0, Inmsg);
    std::cout << "//////////////////////////" << std::endl;


    FirstPlayer.StartingHand();
    SecondPlayer.StartingHand();


    int currentside = 0;
    Player CurrentPlayer = FirstPlayer;
    int MovesAmount = 2;
    int Phase = 0;

    while (pole.CheckLeader()) {
        if (MovesAmount == 0) {
            if (currentside == 0) {
                std::cout << "Ход переходит ко второму игроку" << std::endl;
                currentside = 1;
                CurrentPlayer = SecondPlayer;
            } else {
                std::cout << "Ход переходит ко первому игроку" << std::endl;
                currentside = 0;
                CurrentPlayer = FirstPlayer;
                Phase++;
            }
            if (Phase == 3) {
                Phase = 0;
            }
            MovesAmount = 2;
        }
        if (currentside == 0)
            std::cout << "Ход Первого игрока" << std::endl;
        else
            std::cout << "Ход Второго игрока" << std::endl;
        switch (Phase) {
            case (0): {
                std::cout << "Стадия Авангарда" << std::endl;
                break;
            }
            case (1): {
                std::cout << "Стадия Фланга" << std::endl;
                break;
            }
            case (2): {
                std::cout << "Стадия тыла" << std::endl;
                break;
            }
        }

        std::cout << "//////////////////////////" << std::endl;
        std::cout << "Взять карту(1)" << std::endl;
        std::cout << "Посмотреть руку(2)" << std::endl;
        std::cout << "Получить информацию о герое на поле(3)" << std::endl;
        std::cout << "Атаковать своим героем врага(4)" << std::endl;
        std::cout << "Посмотреть поле(5)" << std::endl;
        std::cout << "Поставить героя(6)" << std::endl;
        std::cout << "Способность(7)" << std::endl;
        std::cout << "Убрать труп(8)" << std::endl;
        std::cout << "//////////////////////////" << std::endl;

        choice = 0;
        lobby->incoming().wait();
        msg = lobby->incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::GamePlayerOptionChoice) {
            msg >> choice;
        } else {
            std::cout << "Error in option choice!\n";
            return;
        }
        switch (choice) {
            case (1): {
                CurrentPlayer.DrawCard();
                MovesAmount--;
                break;
            }
            case (2): {
                CurrentPlayer.GetHand().ShowDeck();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }
            case (3): {
                std::cout << "Введите клетку и линию и сторону" << std::endl;
                int line = 0;
                int cell = 0;
                int side = 0;
                lobby->incoming().wait();
                msg = lobby->incoming().popFront().msg;
                    if (msg.header.id == GameMsgTypes::GameFullCoordinates) {
                    msg >> line >> cell >> side;
                } else {
                    std::cout << "Error in Full Coordinates!\n";
                    return;
                }
                std::cin >> cell >> line >> side;
                Position *kletka = pole.GetPosition(cell, line, side);
                kletka->InfoPosition();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }
            case (4): {
                std::cout << "Введите клетку и линию вашего героя" << std::endl;
                int line = 0;
                int cell = 0;

                LobbyShortCoordinates(line,cell);
                std::vector<bool> CanBeMeleeAttacked;
                CanBeMeleeAttacked = pole.CanBeMeleeAttackedRequest(CurrentPlayer.GetSide()? 0:1);

                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameCanBeMeleeAttacked);
                Attackmsg << CanBeMeleeAttacked;
                CallPechkin(0,Attackmsg);

                Position *YourHero = pole.GetPosition(cell, line, CurrentPlayer.GetSide());

                std::cout << "Введите клетку и линию вражеского героя" << std::endl;
                LobbyShortCoordinates(line,cell);
                int EnemySide;
                if (CurrentPlayer.GetSide() == 0) {
                    EnemySide = 1;
                } else {
                    EnemySide = 0;
                }
                Position *EnemyHero = pole.GetPosition(cell, line, EnemySide);

                if (CurrentPlayer.MeleeAttackCheck(EnemyHero, pole)) {
                    YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());
                    pole.SetPosition(EnemyHero);
                    MovesAmount--;
                    std::vector<Breed> Stats;
                    Stats = ReturnRequest(pole);

                    auto Statsmsg = Message<GameMsgTypes>(GameMsgTypes::GameHeroStats);
                    Statsmsg << Stats;
                    CallPechkin(0,Attackmsg);
                } else {
                    std::cout << "СПЕРЕДИ СТОИТ ДРУГОЙ ГЕРОЙ. НЕЛЬЗЯ АТАКОВАТЬ!!" << std::endl;
                }
                break;
            }
            case (5): {
                pole.Show();
                std::vector<bool> Test;
                Test = pole.CanBeMeleeAttackedRequest(CurrentPlayer.GetSide());
                for (int i = 0; i < Test.size(); i++) {
                    std::cout << std::boolalpha << Test[i] << " ";
                }
                break;
            }
            case (6): {
                std::cout << "Выберете карту" << std::endl;
                lobby->incoming().wait();
                msg = lobby->incoming().popFront().msg;
                if (msg.header.id == GameMsgTypes::GameCardFromHandChoice) {
                    msg >> choice;
                } else {
                    std::cout << "Error in Card from Hand choice\n";
                    return;
                }
                Card ChosenCard = CurrentPlayer.ChooseCard(choice);

                std::cout << "Введите клетку и линию" << std::endl;
                int line = 0;
                int cell = 0;
                LobbyShortCoordinates(line,cell);

                Position *kletka = pole.GetPosition(cell, line, CurrentPlayer.GetSide());
                kletka->SetHero(ChosenCard);
                mapper.MapPowers(kletka->GetHero());
                pole.SetPosition(kletka);
                MovesAmount--;
                break;
            }
            case (7): {
                int cell, line;
                std::cout << "Введите клетку и линию вашего героя: " << std::endl;
               LobbyShortCoordinates(line,cell);
                switch (Phase) {
                    case (0): {
                        Position *Hero = pole.GetPosition(cell, line, currentside);
                        cout << "\nИмя: " << Hero->GetHero().GetName() << "\nСпособность авангарда: "
                             << allCards.GetVector()[Hero->GetHero().GetID() - 1].frontLinePower << "\n";
                        FrontRequest(Hero, pole, currentside);
                        break;
                    }
                    case (1): {
                        Position *Hero = pole.GetPosition(cell, line, currentside);
                        MiddleRequest(Hero, pole, currentside);
                        break;
                    }
                    case (2): {
                        Position *Hero = pole.GetPosition(cell, line, currentside);
                        BackRequest(Hero, pole,currentside);
                        break;;
                    }
                    default: 
                        break;
                }
            }
            case(8):{
                int line,cell=0;
                std::cout << "Введите клетку и линию вашего трупа: " << std::endl;
                LobbyShortCoordinates(line,cell);

                Position *kletka = pole.GetPosition(cell, line, CurrentPlayer.GetSide());
                kletka->RemoveHero();
            }
            default: {
                break;
            }
        }
    }
}

void Game::CallPechkin(int playerId, const Message<GameMsgTypes> &msg){
    lobby->sendToPlayer(playerId, msg);
}
