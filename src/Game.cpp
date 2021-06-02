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

    while (winner != 0 && winner != 1) {

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
                    cout << "\nВЗЯЛ КАРТУ " << std::endl << CurrentPlayer->GetHand().GetVector().back().name << " "
                         << CurrentPlayer->GetHand().GetVector().back().ID <<
                         std::endl;
                    MovesAmount--;

                    auto cardTookMsg = Message<GameMsgTypes>(GameMsgTypes::GameTakeCard);
                    CallPechkin(currentSide ? 0 : 1, cardTookMsg);
                    cout << "Отправлено в ответ на взятие карты" << std::endl;

                }
                break;
            }

            case (GameMsgTypes::GameAttackRequest): {
                pole.Show();
                std::cout << "\nВЫБРАНА АТАКА" << std::endl;

                int line, cell;
                message >> line >> cell;
                cout << "Атакующий " << line << " " << cell << std::endl;

                vector<Breed> before = GetBreeds();
                std::vector<bool> CanBeMeleeAttacked = pole.CanBeMeleeAttackedRequest(currentSide);
                for (int i = 0; i < CanBeMeleeAttacked.size(); i++)
                    if (CanBeMeleeAttacked[i]) {
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
                            cout << "Атакованный " << line << " " << cell << " " << EnemyHero->GetHero().GetName()
                                 << std::endl;
                        }

                        cout << "До атаки " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                             << YourHero->GetHero().GetCurStrength() << std::endl;

                        YourHero->GetHero().Attack(EnemyHero->GetHero(), YourHero->GetHero().GetCurStrength());

                        cout << "После атаки " << "ХП " << EnemyHero->GetHero().GetCurHealth() << " Сила атакующего "
                             << YourHero->GetHero().GetCurStrength() << std::endl;

                        pole.SetPosition(EnemyHero);
                        if (pole.CheckLeader() != 0 && pole.CheckLeader() != 1)
                            SendBreeds(pole, 3);
                        MovesAmount--;
                        break;
                    }

                vector<Breed> after = GetBreeds();
                if (!FieldIsChanged(before, after)) {
                    SendReject();
                }
                break;
            }

            case (GameMsgTypes::GameCardFromHandChoice): {
                std::cout << "\nРЕАЛИЗАЦИЯ КАРТЫ" << std::endl;
                int line, cell;
                message >> choice >> line >> cell;
                cout << "CHOICE " << choice << " CELL " << cell << " LINE " << line << std::endl;

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
                std::cout << "\nСПОСОБНОСТЬ " << std::endl;

                message >> line >> cell;

                Position *Hero = pole.GetPosition(cell, line, currentSide);
                vector<Breed> before = GetBreeds();
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
                vector<Breed> after = GetBreeds();
                if (FieldIsChanged(before, after)) {
                    SendBreeds(pole, 3);
                    MovesAmount--;
                } else {
                    SendReject();
                }
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

                auto canBeRemoved = Message<GameMsgTypes>(GameMsgTypes::GameRemoveBody);
                canBeRemoved << send;
                CallPechkin(currentSide, canBeRemoved);


                std::cout << "УБРАТЬ ТЕЛО" << std::endl;
                int line, cell;

                if (!WaitForMessage()) return;
                auto remove = lobby->incoming().popFront().msg;
                if (remove.header.id == GameMsgTypes::GameRemoveBody) {
                    remove >> line >> cell;
                    cout << "CELL " << cell << " LINE " << line << std::endl;
                }

                Position *chosenCell = pole.GetPosition(cell, line, currentSide);
                chosenCell->RemoveHero();

                SendBreeds(pole, 3);

                MovesAmount--;
                break;
            }
            default: {
                break;
            }
        }
        winner = pole.CheckLeader();
    }


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

bool Game::FieldIsChanged(vector<Breed> before, vector<Breed> after) {
    if (before.size() == after.size())
        for (int i = 0; i < before.size(); i++) {
            cout << before[i].HP << " " << before[i].Strength << " " << before[i].distant << "      " << after[i].HP
                 << " " << after[i].Strength << " " << after[i].distant << "\n";
            if (before[i].HP != after[i].HP ||
                before[i].Strength != after[i].Strength ||
                before[i].distant != after[i].distant)
                return true;
        }
    return false;
}

void Game::CallPechkin(int playerId, const Message<GameMsgTypes> &msg) {

    lobby->sendToPlayer(playerId, msg);
}

vector<Breed> Game::GetBreeds() {
    std::vector<Breed> breeds;
    for (auto &i : pole.GetVector()) {
        Breed stats;
        stats.HP = i->GetHero().GetCurHealth();
        stats.Strength = i->GetHero().GetCurStrength();
        stats.ID = i->GetHero().GetID();
        stats.distant = i->GetHero().CanBeAttackedDistantly();
        breeds.push_back(stats);
    }

    return breeds;
}

void Game::SendBeforeHeroStats() {

    Message<GameMsgTypes> statsMsg(GameMsgTypes::GameBeforeHeroStats);
    CallPechkin(currentSide, statsMsg);
}

void Game::SendReject() {

    Message<GameMsgTypes> reject(GameMsgTypes::GameReject);
    cout << "Отказано\n";
    CallPechkin(currentSide, reject);
}

void Game::SendBreeds(Pole &pole, int numOfPlayers) {

    std::vector<Breed> allStats;
    allStats.resize(0);
    for (auto &i : pole.GetVector()) {
        Breed stats;
        stats.HP = i->GetHero().GetCurHealth();
        stats.Strength = i->GetHero().GetCurStrength();
        stats.ID = i->GetHero().GetID();
        allStats.push_back(stats);
    }

    cout << std::endl;
    Message<GameMsgTypes> statsMsg(GameMsgTypes::GameHeroesStats);
    statsMsg << allStats;

    if (numOfPlayers == 0) {
        CallPechkin(0, statsMsg);
    } else if (numOfPlayers == 1) {
        CallPechkin(1, statsMsg);
    } else {
        CallPechkin(0, statsMsg);
        CallPechkin(1, statsMsg);
    }
}

void Game::ReceiveTarget(int &line, int &cell) {

    if (!WaitForMessage()) return;
    auto choiceMsg = lobby->incoming().popFront().msg;

    if (choiceMsg.header.id == GameMsgTypes::GamePowerRequest) {
        cout << "Получил координаты " << std::endl;
        choiceMsg >> line >> cell;
    }

}

void Game::ReleasePower(int type, Position *position, int side) {

    vector<Hero *> heroes;
    heroes.push_back(&position->GetHero());
    cout << "В вектор добавлен свой " << std::endl;

    int line, cell;
    ReceiveTarget(line, cell);
    Position *chosen_pos = pole.GetPosition(cell, line, side);
    cout << "ХП и сила до способности " << chosen_pos->GetHero().GetCurHealth() << " "
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

    cout << "ХП и сила после способности " << chosen_pos->GetHero().GetCurHealth() << " "
         << chosen_pos->GetHero().GetCurStrength() << std::endl;
}

void Game::FrontRequest(Position *friendPosition) {
    bool done = false;
    switch (friendPosition->GetHero().GetTexture()) {
        case (5): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "   Отправила всех живых врагов" << std::endl;

                    ReleasePower(0, friendPosition, currentSide ? 0 : 1);
                    break;
                }
            break;
        }
        case (6):
        case (8):
        case (3): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых своих" << std::endl;

                    ReleasePower(0, friendPosition, currentSide);
                    break;
                }
            break;
        }
        case (7): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех мертвых своих" << std::endl;

                    ReleasePower(0, friendPosition, currentSide);
                    break;
                }
            break;
        }
        case (1): {
            Interception(friendPosition, pole, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (2): {
            HPFromDead(friendPosition, pole, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (4): {
            InterceptionPlusPower(friendPosition, pole, 0, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (9): {
            PlusStrMinusHP(friendPosition, pole);
            SendBeforeHeroStats();
            break;
        }
        case (11): {
        }
        case (10): {
            Interception(friendPosition, pole, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
    }
    cout << done;
}

void Game::MiddleRequest(Position *friendPosition) {
    bool done = false;
    switch (friendPosition->GetHero().GetTexture()) {
        case (2): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех мертвых своих" << std::endl;

                    ReleasePower(1, friendPosition, currentSide);
                    break;
                }
            break;
        }
        case (6): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    vector<Hero *> heroes;
                    heroes.push_back(&friendPosition->GetHero());

                    int line, cell;
                    ReceiveTarget(line, cell);
                    Position *chosen_pos = pole.GetPosition(cell, line, currentSide);

                    heroes.push_back(&chosen_pos->GetHero());
                    heroes.push_back(&pole.GetPosition(1, 1, currentSide)->GetHero());
                    friendPosition->GetHero().middleLinePower(heroes);
                    break;
                }
            break;
        }
        case (1): {
            vector<bool> result = pole.CanBeMeleeAttackedRequest(currentSide);
            vector<bool> send;
            send.resize(18);
            for (int i = 0 + 9 * currentSide; i < 9 + 9 * currentSide; i++) {
                send[i] = false;
            }
            for (int i = 0 + 9 * (1 - currentSide); i < 9 + 9 * (1 - currentSide); i++) {
                send[i] = result[i - 9 * (1 - currentSide)];
            }
            for (int i = 0; i < send.size(); i++)
                cout << send[i] << " ";
            cout << std::endl;
            for (int i = 0; i < send.size(); i++)
                if (send[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << send;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых врагов" << std::endl;

                    ReleasePower(1, friendPosition, currentSide ? 0 : 1);
                    break;;
                }
            break;
        }
        case (3): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых врагов" << std::endl;

                    ReleasePower(1, friendPosition, currentSide ? 0 : 1);
                    break;
                }
            break;
        }
        case (8): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых врагов" << std::endl;

                    int line, cell;
                    ReceiveTarget(line, cell);
                    Position *chosen_pos = pole.GetPosition(cell, line, currentSide);
                    DamageTwoRows(friendPosition, pole, chosen_pos, done);
                    break;
                }
            break;
        }
        case (4):
        case (11): {
            vector<bool> result = MakeDistantAttack(pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых врагов" << std::endl;

                    ReleasePower(1, friendPosition, currentSide ? 0 : 1);
                    break;
                }
            break;
        }
        case (5): {
            HitAvangard(friendPosition, pole, currentSide, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (9): {
            InterceptionPlusPower(friendPosition, pole, 1, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (10): {
            FrontAndBackGetDamage(friendPosition, pole, currentSide, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
        case (7): {
            PlusPowerByDead(friendPosition, pole, currentSide, done);
            if (done)
                SendBeforeHeroStats();
            break;
        }
    }
    cout << done;
}

void Game::BackRequest(Position *friendPosition) {
    bool done = false;
    switch (friendPosition->GetHero().GetTexture()) {
        case (5): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех живых врагов" << std::endl;

                    int line, cell;
                    ReceiveTarget(line, cell);
                    Position *chosen_pos = pole.GetPosition(cell, line, currentSide);
                    DamageTwoCols(friendPosition, pole, chosen_pos, done);
                    break;
                }
            break;
        }
        case (3): {
            vector<bool> result = SendAllAliveEnemies(friendPosition, pole, currentSide);
            if (friendPosition->GetHero().IsDamaged())
                for (int i = 0; i < result.size(); i++)
                    if (result[i]) {
                        Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                        aliveMsg << result;
                        CallPechkin(currentSide, aliveMsg);

                        std::cout << "Отправила всех живых врагов" << std::endl;

                        ReleasePower(2, friendPosition, currentSide ? 0 : 1);
                        break;
                    }
            break;
        }
        case (2):
        case (9):
        case (8):
        case (10): {
            vector<bool> result = MakeDistantAttack(pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "   Отправила всех живых врагов" << std::endl;

                    ReleasePower(2, friendPosition, currentSide ? 0 : 1);
                    break;
                }
            break;
        }
        case (6): {
            vector<bool> result = SendAllDeadFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "Отправила всех мертвых своих" << std::endl;

                    ReleasePower(2, friendPosition, currentSide);
                    break;
                }
            break;
        }
        case (4):
        case (7):
        case (11): {
            vector<bool> result = SendAllAliveFriends(friendPosition, pole, currentSide);
            for (int i = 0; i < result.size(); i++)
                if (result[i]) {
                    Message<GameMsgTypes> aliveMsg(GameMsgTypes::GamePowerAnswer);
                    aliveMsg << result;
                    CallPechkin(currentSide, aliveMsg);

                    std::cout << "   Отправила всех живых своих" << std::endl;

                    ReleasePower(2, friendPosition, currentSide);
                    break;
                }
            break;
        }
        case (1): {
            DoubleStrengthAgainstLeader(friendPosition, pole, currentSide);
            SendBeforeHeroStats();
            break;
        }
    }
    cout << done;
}

