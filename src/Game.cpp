#include "Game.h"
#include "request.h"

bool Game::WaitForMessage() {
    lobby->incoming().wait();
    if (lobby->incoming().front().msg.header.id == GameMsgTypes::LobbyLeave) {
        return false;
    }
    return true;
}

void Game::SetLeader(std::vector<Player *> &players, int playerside, int choice, Pole &pole, PowerMapper &mapper) {
    Player *CurrentPlayer = players[playerside];
    Position *LeaderPos = pole.GetPosition(1, 1, playerside);
    for (int i = 0; i < CurrentPlayer->GetDeck().GetVector().size(); i++) {
        if (CurrentPlayer->GetDeck().GetVector()[i].ID == choice) {
            LeaderPos->SetHero(CurrentPlayer->GetDeck().GetVector()[i]);
            break;
        }
    }
    mapper.MapPowers(LeaderPos->GetHero());
    LeaderPos->GetHero().MakeLeader();
    pole.SetPosition(LeaderPos);
    CurrentPlayer->GetDeck().GetVector().erase(CurrentPlayer->GetDeck().GetVector().begin() + choice);
    for (int i = 0; i < 2; i++) {
        CurrentPlayer->GetHand().push_back(CurrentPlayer->GetDeck().GetVector().back());
        CurrentPlayer->GetDeck().GetVector().pop_back();
    }
}

void Game::SendBreeds(Pole &pole, int numOfPlayers) {
    std::vector<Breed> allStats;
    allStats.resize(0);
    for (int i = 0; i < pole.GetVector().size(); i++) {
        Breed stats;
        stats.HP = pole.GetVector()[i]->GetHero().GetCurHealth();
        stats.Strength = pole.GetVector()[i]->GetHero().GetCurStrength();
        stats.ID = pole.GetVector()[i]->GetHero().GetID();
        allStats.push_back(stats);
    }

    Message<GameMsgTypes> statsMsg(GameMsgTypes::GameHeroesStats);
    statsMsg << allStats;
    if (numOfPlayers == 0) {
        CallPechkin(0, statsMsg);
        cout << "Бреды отправлены первому" << std::endl;
    } else if (numOfPlayers == 1) {
        CallPechkin(1, statsMsg);
        cout << "Бреды отправлены второму" << std::endl;
    } else {
        CallPechkin(0, statsMsg);
        CallPechkin(1, statsMsg);
        cout << "Бреды отправлены обоим" << std::endl;
    }
}


void Game::StartGame() {
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
    std::vector<Player *> Players(0);
    Players.push_back(&(FirstPlayer));
    Players.push_back(&(SecondPlayer));
    std::vector<Card> StepaFirstAllCards = FirstPlayerDeck.GetVector();
    auto msg = Message<GameMsgTypes>(GameMsgTypes::GameDeck);
    msg << StepaFirstAllCards;
    CallPechkin(0, msg);

    std::vector<Card> StepaSecondAllCards = SecondPlayerDeck.GetVector();
    msg = Message<GameMsgTypes>(GameMsgTypes::GameDeck);
    msg << StepaSecondAllCards;
    CallPechkin(1, msg);

    CallPechkin(0, msg);
    msg << StepaFirstAllCards;
    CallPechkin(1, msg);

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
    std::cout << "Выбери лидера Второй игрок" << std::endl;
    for (int i = 17; i < SecondPlayerDeck.GetVector().size(); i++) {
        std::cout << SecondPlayerDeck.GetVector()[i].name << " " << "Здоровье Героя:"
                  << SecondPlayerDeck.GetVector()[i].HP << " " << "Атака Героя:"
                  << SecondPlayerDeck.GetVector()[i].strength << std::endl;
    }
    int choice, playerside;
    int x, y;
    std::cout << "////" << std::endl;
    if (!WaitForMessage()) return;
    auto leadmsg = lobby->incoming().popFront().msg;
    if (leadmsg.header.id == GameMsgTypes::GameCardFromHandChoice) {
        leadmsg >> choice >> x >> y >> playerside;
    } else {
        std::cout << "Error in leader choice!\n";
        return;
    }
    std::cout << "CHOICE " << choice << " PLAYERSIDE " << playerside << std::endl;
    SetLeader(Players, playerside, choice, pole, mapper);

    if (!WaitForMessage()) return;
    leadmsg = lobby->incoming().popFront().msg;
    if (leadmsg.header.id == GameMsgTypes::GameCardFromHandChoice) {
        leadmsg >> choice >> x >> y >> playerside;
    } else {
        std::cout << "Error in leader choice!\n";
        return;
    }
    std::cout << "CHOICE " << choice << " PLAYERSIDE " << playerside << std::endl;
    SetLeader(Players, playerside, choice, pole, mapper);

    cout << " Лидер 1 ";
    pole.GetPosition(1, 1, 0)->InfoPosition();
    cout << " Лидер 2 ";
    pole.GetPosition(1, 1, 1)->InfoPosition();

    // Breed allStats[pole.GetVector().size()];

    SendBreeds(pole, 3);

    // FirstPlayer.StartingHand();
    // SecondPlayer.StartingHand();


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

        /* std::cout << "//////////////////////////" << std::endl;
         std::cout << "Взять карту(1)" << std::endl;
         std::cout << "Посмотреть руку(2)" << std::endl;
         std::cout << "Получить информацию о герое на поле(3)" << std::endl;
         std::cout << "Атаковать своим героем врага(4)" << std::endl;
         std::cout << "Посмотреть поле(5)" << std::endl;
         std::cout << "Поставить героя(6)" << std::endl;
         std::cout << "Способность(7)" << std::endl;
         std::cout << "Убрать труп(8)" << std::endl;
         std::cout << "//////////////////////////" << std::endl;*/

        choice = 0;
        if (!WaitForMessage()) return;
        msg = lobby->incoming().popFront().msg;
        cout << "ПРИШЛО" << std::endl;
        switch (msg.header.id) {
            /*case (): {
                if(!(CurrentPlayer.GetHand().GetVector().size() >=5)){
                    CurrentPlayer.DrawCard();
                    MovesAmount--;
                }
                break;
            }*/
            /*case (2): {
                CurrentPlayer.GetHand().ShowDeck();
                std::cout << "//////////////////////////" << std::endl;
                break;
            }*/
            /*case (3): {
                std::cout << "Введите клетку и линию и сторону" << std::endl;
                int line = 0;
                int cell = 0;
                int side = 0;
                if(!WaitForMessage()) return;
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
            }*/
            case (GameMsgTypes::GameAttackRequest): {
                pole.Show();
                std::cout << "ВЫБРАНА АТАКА" << std::endl;
                int line = 0;
                int cell = 0;

                msg >> line >> cell;
                cout << "АТАКУЮЩИЙ " << line << " " << cell << std::endl;
                std::vector<bool> CanBeMeleeAttacked;
                CanBeMeleeAttacked = pole.CanBeMeleeAttackedRequest(currentside);

                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameCanBeAttacked);
                Attackmsg << CanBeMeleeAttacked;
                CallPechkin(currentside, Attackmsg);
                for (int i = 0; i < 9; i++)
                    cout << CanBeMeleeAttacked[i] << " ";
                cout << std::endl;

                Position *YourHero = pole.GetPosition(cell, line, CurrentPlayer.GetSide());
                int EnemySide;
                if (CurrentPlayer.GetSide() == 0) {
                    EnemySide = 1;
                } else {
                    EnemySide = 0;
                }

                Position *EnemyHero;

                if (!WaitForMessage()) return;

                auto attackerMsg = lobby->incoming().popFront().msg;

                if (msg.header.id == GameMsgTypes::GameAttackRequest) {
                    attackerMsg >> line >> cell;
                    EnemyHero = pole.GetPosition(cell, line, currentside ? 0 : 1);
                    cout << "АТАКОВАННЫЙ " << line << " " << cell << " " << EnemyHero->GetHero().GetName() << std::endl;
                }

                if (CurrentPlayer.MeleeAttackCheck(EnemyHero, pole)) {
                    cout << "ДО АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                         << YourHero->GetHero().GetCurStrength() << std::endl;
                    
                    YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());

                    cout << "ПОСЛЕ АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                         << YourHero->GetHero().GetCurStrength() << std::endl;
                    pole.SetPosition(EnemyHero);
                    MovesAmount--;
                    std::vector<Breed> Stats;
                    Stats = ReturnRequest(pole);

                    auto Statsmsg = Message<GameMsgTypes>(GameMsgTypes::GameHeroesStats);
                    Statsmsg << Stats;
                    CallPechkin(0, Attackmsg);
                } else {
                    std::cout << "СПЕРЕДИ СТОИТ ДРУГОЙ ГЕРОЙ. НЕЛЬЗЯ АТАКОВАТЬ!!" << std::endl;
                }
                MovesAmount--;
                break;
            }
                /*case (5): {
                    pole.Show();
                    std::vector<bool> Test;
                    Test = pole.CanBeMeleeAttackedRequest(CurrentPlayer.GetSide());
                    for (int i = 0; i < Test.size(); i++) {
                        std::cout << std::boolalpha << Test[i] << " ";
                    }
                    break;
                }*/
            case (GameMsgTypes::GameCardFromHandChoice): {
                std::cout << "РЕАЛИЗАЦИЯ КАРТЫ" << std::endl;
                int line = 0;
                int cell = 0;
                msg >> choice >> line >> cell;
                cout << " CHOICE " << choice << " CELL " << cell << " LINE " << line << std::endl;
                Card ChosenCard = CurrentPlayer.ChooseCard(choice);

                Position *kletka = pole.GetPosition(cell, line, currentside);
                kletka->SetHero(ChosenCard);
                mapper.MapPowers(kletka->GetHero());
                pole.SetPosition(kletka);
                MovesAmount--;
                pole.Show();
                SendBreeds(pole, currentside ? 0 : 1);
                break;
            }
                /*case (7): {
                    int cell, line;
                    std::cout << "Введите клетку и линию вашего героя: " << std::endl;
                    LobbyShortCoordinates(line, cell);
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
                            BackRequest(Hero, pole, currentside);
                            break;;
                        }
                        default:
                            break;
                    }
                }*/
                /*case (8): {
                    int line, cell = 0;
                    std::cout << "Введите клетку и линию вашего трупа: " << std::endl;
                    LobbyShortCoordinates(line, cell);

                    Position *kletka = pole.GetPosition(cell, line, CurrentPlayer.GetSide());
                    kletka->RemoveHero();
                }*/
            default: {
                break;
            }
        }
    }
}

void Game::CallPechkin(int playerId, const Message<GameMsgTypes> &msg) {
    lobby->sendToPlayer(playerId, msg);
}
