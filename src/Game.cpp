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

    allCards = sql.GetCards();

    Deck FirstPlayerDeck(allCards.GetVector());
    Deck SecondPlayerDeck(allCards.GetVector());

    Player *FirstPlayer = new Player(FirstPlayerDeck, 0);
    Player *SecondPlayer = new Player(SecondPlayerDeck, 1);
    std::vector<Player *> Players;
    Players.push_back(FirstPlayer);
    Players.push_back(SecondPlayer);

    std::vector<Card> firstDeck = FirstPlayerDeck.GetVector();
    auto deckMsg = Message<GameMsgTypes>(GameMsgTypes::GameDeck);
    deckMsg << firstDeck;
    CallPechkin(0, deckMsg);

    std::vector<Card> secondDeck = SecondPlayerDeck.GetVector();
    deckMsg << secondDeck;
    CallPechkin(1, deckMsg);

    CallPechkin(0, deckMsg);
    deckMsg << firstDeck;
    CallPechkin(1, deckMsg);

    int choice;
    int x, y;

    for (int i = 0; i < 2; i++) {
        if (!WaitForMessage()) return;
        auto leadMsg = lobby->incoming().popFront().msg;
        if (leadMsg.header.id == GameMsgTypes::GameCardFromHandChoice) {
            leadMsg >> choice >> x >> y >> currentSide;
        } else {
            std::cout << "Error in leader choice!\n";
            return;
        }
        std::cout << "CHOICE " << choice << " PLAYERSIDE " << currentSide << std::endl;
        SetLeader(Players, currentSide, choice, pole, mapper);
    }

    cout << " Лидер 1 ";
    pole.GetPosition(1, 1, 0)->InfoPosition();
    cout << " Лидер 2 ";
    pole.GetPosition(1, 1, 1)->InfoPosition();

    SendBreeds(pole, 3);

    currentSide = 0;
    Player *CurrentPlayer = FirstPlayer;
    int MovesAmount = 2;
    int Phase = 0;

    while (pole.CheckLeader() != 0 && pole.CheckLeader() != 1) {

        if (MovesAmount == 0) {
            if (currentSide == 0) {
                std::cout << "Ход переходит ко второму игроку" << std::endl;
                currentSide = 1;
                CurrentPlayer = SecondPlayer;
            } else {
                std::cout << "Ход переходит ко первому игроку" << std::endl;
                currentSide = 0;
                CurrentPlayer = FirstPlayer;
                Phase++;
            }
            if (Phase == 3) {
                Phase = 0;
            }
            MovesAmount = 2;
        }

        if (currentSide == 0)
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
            default: {
                cout << "Phase error!";
                return;
            }
        }

        if (!WaitForMessage()) return;
        auto message = lobby->incoming().popFront().msg;

        switch (message.header.id) {

            case (GameMsgTypes::GameTakeCard): {
                if (CurrentPlayer->GetHand().GetVector().size() < 5) {

                    CurrentPlayer->DrawCard();
                    cout << " ВЗЯЛ КАРТУ " << std::endl << CurrentPlayer->GetHand().GetVector().back().name << " "
                         << CurrentPlayer->GetHand().GetVector().back().ID <<
                         std::endl;
                    MovesAmount--;

                    cout << std::endl;
                    auto cardTookMsg = Message<GameMsgTypes>(GameMsgTypes::GameTakeCard);
                    CallPechkin(currentSide ? 0 : 1, cardTookMsg);
                    cout << " Отправлено в ответ на взятие карты " << std::endl;

                }
                break;
            }

            case (GameMsgTypes::GameAttackRequest): {
                pole.Show();
                std::cout << "ВЫБРАНА АТАКА" << std::endl;

                int line, cell;
                message >> line >> cell;
                cout << "АТАКУЮЩИЙ " << line << " " << cell << std::endl;

                std::vector<bool> CanBeMeleeAttacked;
                CanBeMeleeAttacked = pole.CanBeMeleeAttackedRequest(currentSide);
                /*if (for_each(CanBeMeleeAttacked.begin(), CanBeMeleeAttacked.end(),
                             [](bool position) -> bool {
                                 return !position;
                             })) {
                    cout << "HERE";
                    auto attackMsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                    CallPechkin(currentSide, attackMsg);
                    break;
                }*/
                auto attackMsg = Message<GameMsgTypes>(GameMsgTypes::GameCanBeAttacked);
                attackMsg << CanBeMeleeAttacked;
                CallPechkin(currentSide, attackMsg);


                Position *YourHero = pole.GetPosition(cell, line, currentSide);

                Position *EnemyHero;

                if (!WaitForMessage()) return;
                auto attackerMsg = lobby->incoming().popFront().msg;

                if (attackerMsg.header.id == GameMsgTypes::GameAttackRequest) {
                    attackerMsg >> line >> cell;
                    EnemyHero = pole.GetPosition(cell, line, currentSide ? 0 : 1);
                    cout << "АТАКОВАННЫЙ " << line << " " << cell << " " << EnemyHero->GetHero().GetName() << std::endl;
                }

                cout << "ДО АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << "Сила атакующего "
                     << YourHero->GetHero().GetCurStrength() << std::endl;

                YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());

                cout << "ПОСЛЕ АТАКИ " << "ХП " << EnemyHero->GetHero().GetCurHealth() << "Сила атакующего "
                     << YourHero->GetHero().GetCurStrength() << std::endl;

                pole.SetPosition(EnemyHero);
                if (pole.CheckLeader() != 0 && pole.CheckLeader() != 1)
                    SendBreeds(pole, 3);
                MovesAmount--;
                break;
            }

            case (GameMsgTypes::GameCardFromHandChoice): {
                std::cout << "РЕАЛИЗАЦИЯ КАРТЫ" << std::endl;
                int line, cell;
                message >> choice >> line >> cell;
                cout << " CHOICE " << choice << " CELL " << cell << " LINE " << line << std::endl;

                Position *chosenCell = pole.GetPosition(cell, line, currentSide);
                for (int i = 0; i < CurrentPlayer->GetHand().GetVector().size(); i++) {
                    if (CurrentPlayer->GetHand().GetVector()[i].ID == choice) {
                        chosenCell->SetHero(CurrentPlayer->GetHand().GetVector()[i]);
                        CurrentPlayer->GetHand().GetVector().erase(CurrentPlayer->GetHand().GetVector().begin() + i);
                    }
                }

                mapper.MapPowers(chosenCell->GetHero());
                pole.SetPosition(chosenCell);
                MovesAmount--;

                pole.Show();
                Message<GameMsgTypes> released(GameMsgTypes::GameCardReleased);
                released << chosenCell->GetHero().GetID() << chosenCell->GetHero().GetCurHealth()
                         << chosenCell->GetHero().GetCurStrength() << line << cell;
                CallPechkin(currentSide ? 0 : 1, released);

                break;
            }

            case (GameMsgTypes::GamePowerRequest): {
                int cell, line;
                std::cout << " СПОСОБНОСТЬ " << std::endl;

                message >> line >> cell;

                Position *Hero = pole.GetPosition(cell, line, currentSide);
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
                    default: {
                        cout << "Error in powers" << std::endl;
                        break;
                    }
                }
                MovesAmount--;

                break;
            }

            case (GameMsgTypes::GameRemoveBodyRequest): {

                vector<bool> dead = SendAllDeadFriends(NULL, pole, currentSide);

                cout << std::endl;
                vector<bool> send;
                send.clear();

                if (currentSide == 0) {
                    for (int i = 0; i < 9; i++) {
                        send.push_back(dead[i]);
                    }
                } else {
                    for (int i = 9; i < 18; i++) {
                        send.push_back(dead[i]);
                    }
                }

                if (for_each(send.begin(), send.end(),
                             [](bool position) -> bool { return !position; })) {
                    auto canBeRemoved = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                    CallPechkin(currentSide, canBeRemoved);
                    break;
                } else {
                    auto canBeRemoved = Message<GameMsgTypes>(GameMsgTypes::GameRemoveBody);
                    canBeRemoved << send;
                    CallPechkin(currentSide, canBeRemoved);
                }


                std::cout << "УБРАТЬ ТЕЛО" << std::endl;
                int line = 0;
                int cell = 0;

                if (!WaitForMessage()) return;

                auto remove = lobby->incoming().popFront().msg;
                if (remove.header.id == GameMsgTypes::GameRemoveBody)
                    remove >> line >> cell;

                cout << "CELL " << cell << " LINE " << line << std::endl;

                Position *kletka = pole.GetPosition(cell, line, currentSide);
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
    CallPechkin(currentSide, statsMsg);
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
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (11): {
            vector<bool> result = MakeDistantAttack(pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (6): {

        }
        case (8): {

        }
        case (3): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentSide);
            break;
        }
        case (7): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех мертвых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentSide);
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
        case (2): {

        }
        case (6): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех мертвых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentSide);
            break;
        }
        case (1): {

        }
        case (3): {

        }
        case (8): {
            std::cout << "Зашёл в метод" << std::endl;
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            if (result.empty()) {
                std::cout << "Пустой вектор" << std::endl;
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            std::cout << "Перед отправкой" << std::endl;
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (4): {

        }
        case (11): {
            vector<bool> result = MakeDistantAttack(pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(1, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (5): {
            HitAvangard(friendPosition, pole, currentSide);
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
            FrontAndBackGetDamage(friendPosition, pole, currentSide);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
    }
}

void Game::BackRequest(Position *friendPosition) {
    switch (friendPosition->GetHero().GetTexture()) {
        case (3): {

        }
        case (5): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (2): {

        }
        case (9): {

        }
        case (10): {
            vector<bool> result = MakeDistantAttack(pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(0, friendPosition, currentSide ? 0 : 1);
            break;
        }
        case (4): {

        }
        case (6): {

        }
        case (7): {

        }
        case (11): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentSide);
            if (result.empty()) {
                auto Attackmsg = Message<GameMsgTypes>(GameMsgTypes::GameReject);
                CallPechkin(currentSide, Attackmsg);
                break;
            }
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых своих" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentSide);
            break;
        }
        case (1): {
            DoubleStrengthAgainstLeader(friendPosition, pole, currentSide);
            SendBeforeHeroStats();
            SendBreeds(pole, 3);
            break;
        }
        case (8): {
            vector<bool> result = pole.CanBeMeleeAttackedRequest(currentSide);
            Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
            aliveMsg << result;
            CallPechkin(currentSide, aliveMsg);

            std::cout << "   Отправила всех живых врагов" << std::endl;
            for (auto &&i : result) {
                std::cout << i << " ";
            }
            std::cout << std::endl;

            ReleasePower(2, friendPosition, currentSide ? 0 : 1);
            break;
        }
    }
}

