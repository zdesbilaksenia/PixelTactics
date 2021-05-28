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

void Game::StartGame() {
    Position FirstPlayerLeader(1, 1, 0);
    Position SecondPlayerLeader(1, 1, 1);

    MySQL sql;
    Deck allCards;
    //Deck CheatDeck;
    allCards = sql.GetCards();
    //CheatDeck.push_back(allCards.GetVector()[10]);
    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());
    Player *FirstPlayer = new Player(FirstPlayerDeck, 0);
    Player *SecondPlayer = new Player(SecondPlayerDeck, 1);
    std::vector<Player *> Players(0);
    Players.push_back(FirstPlayer);
    Players.push_back(SecondPlayer);
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

    currentside = 0;
    Player *CurrentPlayer = FirstPlayer;
    int MovesAmount = 2;
    int Phase = 0;

    while (pole.CheckLeader() != 0 && pole.CheckLeader() != 1) {
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

        choice = 0;
        if (!WaitForMessage()) return;
        msg = lobby->incoming().popFront().msg;
        cout << "ПРИШЛО" << std::endl;
        switch (msg.header.id) {
            case (GameMsgTypes::GameTakeCard): {
                if (CurrentPlayer->GetHand().GetVector().size() < 5) {
                    CurrentPlayer->DrawCard();
                    MovesAmount--;
                    cout << " ВЗЯЛ КАРТУ " << std::endl << CurrentPlayer->GetHand().GetVector().back().name << " "
                         << CurrentPlayer->GetHand().GetVector().back().ID <<
                         std::endl;
                    for (int i = 0; i < CurrentPlayer->GetHand().GetVector().size(); i++) {
                        cout << CurrentPlayer->GetHand().GetVector()[i].ID << " ";
                    }
                    cout << std::endl;
                    auto cardTookMsg = Message<GameMsgTypes>(GameMsgTypes::GameTakeCard);
                    CallPechkin(currentside ? 0 : 1, cardTookMsg);
                    cout << " Отправлено в ответ на взятие карты " << std::endl;
                }
                break;
            }
            case (GameMsgTypes::GameAttackRequest): {
                pole.Show();
                std::cout << "ВЫБРАНА АТАКА" << std::endl;
                int line = 0;
                int cell = 0;

                msg >> line >> cell;
                cout << "АТАКУЮЩИЙ " << line << " " << cell << std::endl;
                std::vector<bool> CanBeMeleeAttacked;
                CanBeMeleeAttacked = pole.CanBeMeleeAttackedRequest(currentside);

                if (CanBeMeleeAttacked.size() != 0) {
                    auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameCanBeAttacked);
                    Attackmsg << CanBeMeleeAttacked;
                    CallPechkin(currentside, Attackmsg);
                } else {
                    auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                    CallPechkin(currentside, Attackmsg);
                    break;
                }

                Position *YourHero = pole.GetPosition(cell, line, CurrentPlayer->GetSide());
                int EnemySide;
                if (CurrentPlayer->GetSide() == 0) {
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

                cout << "ДО АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                     << YourHero->GetHero().GetCurStrength() << std::endl;

                YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());

                cout << "ПОСЛЕ АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                     << YourHero->GetHero().GetCurStrength() << std::endl;
                pole.SetPosition(EnemyHero);
                if (pole.CheckLeader() != 0 && pole.CheckLeader() != 1)
                    SendBreeds(pole, 3);
                MovesAmount--;

                break;
            }
            case (GameMsgTypes::GameCardFromHandChoice): {
                std::cout << "РЕАЛИЗАЦИЯ КАРТЫ" << std::endl;
                int line = 0;
                int cell = 0;
                msg >> choice >> line >> cell;
                cout << " CHOICE " << choice << " CELL " << cell << " LINE " << line << std::endl;
                //Card ChosenCard = CurrentPlayerChooseCard(choice);
                Position *kletka = pole.GetPosition(cell, line, currentside);

                for (int i = 0; i < CurrentPlayer->GetHand().GetVector().size(); i++) {
                    if (CurrentPlayer->GetHand().GetVector()[i].ID == choice) {
                        kletka->SetHero(CurrentPlayer->GetHand().GetVector()[i]);
                        CurrentPlayer->GetHand().GetVector().erase(CurrentPlayer->GetHand().GetVector().begin() + i);
                    }
                }
                mapper.MapPowers(kletka->GetHero());
                pole.SetPosition(kletka);
                cout << kletka->GetHero().GetID() << std::endl;
                MovesAmount--;
                for (int i = 0; i < CurrentPlayer->GetHand().GetVector().size(); i++) {
                    cout << CurrentPlayer->GetHand().GetVector()[i].ID << " ";
                }
                pole.Show();
                Message<GameMsgTypes> released(GameMsgTypes::GameCardReleased);
                released << kletka->GetHero().GetID() << kletka->GetHero().GetCurHealth()
                         << kletka->GetHero().GetCurStrength() << line << cell;
                CallPechkin(currentside ? 0 : 1, released);
                break;
            }
            case (GameMsgTypes::GamePowerRequest): {
                int cell, line;
                std::cout << " СПОСОБНОСТЬ " << std::endl;

                msg >> line >> cell;
                std::cout <<"x,y " << line << cell << std::endl;
                Position *Hero = pole.GetPosition(cell, line, currentside);
                switch (Phase) {
                    case (0): {
                        cout << "\nИмя: " << Hero->GetHero().GetName() << "\nСпособность авангарда: "
                             << allCards.GetVector()[Hero->GetHero().GetTexture() - 1].frontLinePower << "\n";
                        FrontRequest(Hero);
                        break;
                    }
                    case (1): {
                        cout << "\nИмя: " << Hero->GetHero().GetName() << "\nСпособность фланга: "
                             << allCards.GetVector()[Hero->GetHero().GetTexture() - 1].middleLinePower << "\n";
                        MiddleRequest(Hero);
                        break;
                    }
                    case (2): {
                        cout << "\nИмя: " << Hero->GetHero().GetName() << "\nСпособность тыла: "
                             << allCards.GetVector()[Hero->GetHero().GetTexture() - 1].backLinePower << "\n";
                        BackRequest(Hero);
                        break;
                    }
                    default:
                        break;
                }
                MovesAmount--;

                break;
            }
            case (GameMsgTypes::GameRemoveBodyRequest): {
                vector<bool> dead = SendAllDeadFriends(NULL, pole, currentside);

                for (int i = 0; i < dead.size(); i++) {
                    cout << dead[i] << " ";
                }

                cout << std::endl;
                vector<bool> send;
                send.clear();

                if (currentside == 0) {
                    for (int i = 0; i < 9; i++) {
                        send.push_back(dead[i]);
                    }
                } else {
                    for (int i = 9; i < 18; i++) {
                        send.push_back(dead[i]);
                    }
                }

                Message<GameMsgTypes> d(GameMsgTypes::GameRemoveBody);
                d << send;
                CallPechkin(currentside, d);


                std::cout << "УБРАТЬ ТЕЛО" << std::endl;
                int line = 0;
                int cell = 0;

                if (!WaitForMessage()) return;

                auto remove = lobby->incoming().popFront().msg;
                if (remove.header.id == GameMsgTypes::GameRemoveBody)
                    remove >> line >> cell;

                cout << " CELL " << cell << " LINE " << line << std::endl;

                Position *kletka = pole.GetPosition(cell, line, currentside);
                kletka->RemoveHero();

                SendBreeds(pole, 3);

                MovesAmount--;
                break;
            }
            default: {
                break;
            }
        }
    }

    int winner = pole.CheckLeader();

    if (winner == 0) {
        Message<GameMsgTypes> won(GameMsgTypes::GameWon);
        CallPechkin(0, won);
        Message<GameMsgTypes> lost(GameMsgTypes::GameLost);
        CallPechkin(1, lost);
    } else if (winner == 1) {
        Message<GameMsgTypes> won(GameMsgTypes::GameWon);
        CallPechkin(1, won);
        Message<GameMsgTypes> lost(GameMsgTypes::GameLost);
        CallPechkin(0, lost);
    }

}

void Game::CallPechkin(int playerId, const Message<GameMsgTypes> &msg) {
    lobby->sendToPlayer(playerId, msg);
}

void Game::SendBeforeHeroStats() {
    Message<GameMsgTypes> statsMsg(GameMsgTypes::GameBeforeHeroStats);
    CallPechkin(currentside, statsMsg);
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

    for (int i = 0; i < allStats.size(); i++) {
        cout << allStats[i].ID << " ";
    }
    cout << std::endl;
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

void Game::RecieveTarget(int &line, int &cell) {
    if (!WaitForMessage()) return;
    auto choiceMsg = lobby->incoming().popFront().msg;

    if (choiceMsg.header.id == GameMsgTypes::GamePowerRequest) {
        cout << "Получил координаты челика " << std::endl;
        choiceMsg >> line >> cell;
    }

}

void Game::ReleasePower(int type, Position *position, int side) {
    vector<Hero *> heroes;
    heroes.push_back(&position->GetHero());
    cout << "В вектор добавлен свой " << std::endl;
    int line, cell;
    RecieveTarget(line, cell);
    cout << " Пришел челик " << line << " " << cell << std::endl;
    Position *chosen_pos = pole.GetPosition(cell, line, side);
    cout << " ХП И СИЛА ДО СПОСОБНОСТИ " << chosen_pos->GetHero().GetCurHealth() << " "
         << chosen_pos->GetHero().GetCurStrength() << std::endl;
    heroes.push_back(&(chosen_pos->GetHero()));
    cout << "В вектор добавлен противник " << std::endl;
    switch (type) {
        case 0: {
            position->GetHero().frontLinePower(heroes);
            break;
        }
        case 1: {
            position->GetHero().middleLinePower(heroes);
            break;
        }
        case 2: {
            position->GetHero().backLinePower(heroes);
            break;
        }
        default:
            break;
    }

    cout << " ХП И СИЛА ПОСЛЕ СПОСОБНОСТИ " << chosen_pos->GetHero().GetCurHealth() << " "
         << chosen_pos->GetHero().GetCurStrength() << std::endl;
    SendBreeds(pole, 3);
}

void Game::FrontRequest(Position *friendPosition) {
    switch (friendPosition->GetHero().GetTexture()) {
        case (5): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentside ? 0 : 1);
            break;
        }
        case (11): {
            vector<bool> result = MakeDistantAttack(pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentside ? 0 : 1);
            break;
        }
        case(6):{

        }
        case(8):{

        }
        case (3): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentside);
            break;
        }
        case (7): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех мертвых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentside);
            break;
        }
        case (1): {
            Interception(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (2): {
            HPFromDead(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (4): {
            InterceptionPlusPower(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (9): {
            PlusStrMinusHP(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (10): {
            Interception(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
    }

}

void Game::MiddleRequest(Position *friendPosition) {
    std::cout << "MiddleReq" << friendPosition->GetHero().GetTexture() << std::endl;
    switch (friendPosition->GetHero().GetTexture()) {
        case(2):{

        }
        case (6): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех мертвых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentside);
            break;
        }
        case(1):{

        }
        case(3):{

        }
        case (8): {
            std::cout<< "Зашёл в метод" << std::endl;
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentside);
            if (result.empty()) {
                std::cout << "Пустой вектор" << std::endl;
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            std::cout << "Перед отправкой" << std::endl;
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentside ? 0 : 1);
            break;
        }
        case(4):{

        }
        case (11): {
            vector<bool> result = MakeDistantAttack(pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentside ? 0 : 1);
            break;
        }
        case (5): {
            HitAvangard(friendPosition, pole, currentside);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (9): {
            InterceptionPlusPower(friendPosition, pole);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (10): {
            FrontAndBackGetDamage(friendPosition, pole, currentside);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
    }
}

void Game::BackRequest(Position *friendPosition) {
    switch (friendPosition->GetHero().GetTexture()) {
        case(3):{

        }
        case (5): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentside ? 0 : 1);
            break;
        }
        case(2):{

        }
        case(9):{

        }
        case (10): {
            vector<bool> result = MakeDistantAttack(pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentside ? 0 : 1);
            break;
        }
        case(4):{

        }
        case(6):{

        }
        case(7):{

        }
        case (11): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentside);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentside, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentside);
            break;
        }
        case (1): {
            DoubleStrengthAgainstLeader(friendPosition, pole, currentside);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (8): {
            vector<bool> result = pole.CanBeMeleeAttackedRequest(currentside);
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentside, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentside ? 0 : 1);
            break;
        }
    }
}

