#include <iostream>
#include <vector>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Button.h"
#include "Command.h"
#include "Commands/CommandAttack.h"
#include "Commands/CommandTakeCard.h"
#include "Card.h"
#include "Tile.h"
#include "configurations.cpp"

class CommandMakeLobby : public Command
{
public:
    CommandMakeLobby(GameTcpClient *_client, bool &_lobbyWasCreated) : lobbyWasCreated(_lobbyWasCreated), client(_client) {}
    void execute() override
    {
        if (client->isConnected())
        {
            cout << "CommandMakeLobby::execute() : client is connected!!" << endl;
            client->makeLobby();
            client->incoming().wait();
            auto msg = client->incoming().popFront().msg;
            cout << msg << endl;
            if (msg.header.id == GameMsgTypes::LobbyWaitingForPlayer)
            {
                cout << "CommandMakeLobby::execute() : Lobby waiting for player" << endl;
                client->incoming().wait();
                msg = client->incoming().popFront().msg;
                if (msg.header.id == GameMsgTypes::LobbyGameStart)
                {
                    cout << "Lobby Game started!!!" << endl;
                }
            }
            else
            {
                cout << "Didn't recieve LobbyWaitingForPlayer" << endl;
            }
        }
    }

    ~CommandMakeLobby() { client = nullptr; }

private:
    bool lobbyWasCreated;
    GameTcpClient *client;
};

bool menu(RenderWindow &window,
          Mouse &mouse,
          Event &event,
          GameTcpClient *client)
{
    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/low_panel.png");
    RectangleShape backgroundRect;
    backgroundRect.setSize(Vector2f(windowWidth, windowHeight));
    backgroundRect.setTexture(&backgroundTx);
    backgroundRect.setPosition(Vector2f(0, 0));

    bool lobbyWasCreated = false;
    CommandMakeLobby cmdMakeLobby(client, lobbyWasCreated);
    //CommandStringTest cmdMakeLobby("Connecting to lobby");
    CommandStringTest cmdConnectToLobby("Button2 : Connecting to lobby");

    Texture txMakeLobby;
    txMakeLobby.loadFromFile("../img/make_lobby.png");
    Button btnMakeLobby(window, mouse, &cmdMakeLobby);
    btnMakeLobby.setTexture(&txMakeLobby);

    Button btnConnectToLobby(window, mouse, &cmdConnectToLobby);

    btnMakeLobby.setColors(Color::Blue, Color::Magenta, Color::Green);
    btnConnectToLobby.setColors(Color::Blue, Color::Magenta, Color::Green);
    btnMakeLobby.setPosition(100, 100);
    btnConnectToLobby.setPosition(100, 200);

    vector<Button *> buttons = {&btnMakeLobby, &btnConnectToLobby};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            //Если нажали на кнопку на мыши
            case (Event::MouseButtonPressed):
            {
                buttonsManager.mouseIsPressed();
                if (lobbyWasCreated)
                {
                    cout << "Lobby was created!!!" << endl;
                    return 1;
                }
                break;
            }
            case (Event::MouseButtonReleased):
            {
                buttonsManager.mouseIsReleased();
            }
            case (Event::MouseMoved):
            {
                buttonsManager.updateFocus();
                break;
            }
            //Закрытие окна
            case (Event::Closed):
            {
                window.close();
            }
            default:
                break;
            }
        }

        window.clear();

        window.draw(backgroundRect);
        buttonsManager.draw();

        window.display();
    }
    return 0;
}

//Чтобы визуально сжать код
#define GAME_ELEMENTS 1

bool otherPlayersTurn(RenderWindow &window,
                      Mouse &mouse,
                      Event &event,
                      GameTcpClient *client,
                      TilesManager &playerTilesManager,
                      TilesManager &opponentTilesManager,
                      CardsManager &cardsManager,
                      ButtonsManager &buttonsManager,
                      RectangleShape &backgroundRect,
                      RectangleShape &lowerPanelRect)
{
    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            //Если двигаем мышкой
            case (Event::MouseMoved):
            {
                buttonsManager.updateFocus();
                playerTilesManager.updateFocus();
                opponentTilesManager.updateFocus();
                cardsManager.updateFocus();
                break;
            }

            //ИСПРАВИТЬ!!!
            //Необходимо ждать ответ от сервера
            case (Event::KeyPressed):
            {
                if (Keyboard::isKeyPressed(Keyboard::A))
                {
                    return 1;
                }
                break;
            }
            //

            //Закрытие окна
            case (Event::Closed):
            {
                window.close();
                return 0;
                break;
            }
            default:
                break;
            }
        }

        window.clear();

        window.draw(backgroundRect);
        window.draw(lowerPanelRect);

        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();
    }
}

//Потом разбить на стадии игры (начало, авангард и т.п.)
void game(RenderWindow &window,
          Mouse &mouse,
          Event &event,
          GameTcpClient *client)
{

#if GAME_ELEMENTS == 1

    TilesManager playerTilesManager(window, mouse, Side::sidePlayer);
    TilesManager opponentTilesManager(window, mouse, Side::sideOpponent);

    //Текстура для союзных тайлов
    Texture playerTileTx;
    playerTileTx.loadFromFile("../img/tx_2.png");
    playerTilesManager.setTexture(&playerTileTx);
    //Текстура для тайлов соперника
    Texture opponentTileTx;
    opponentTileTx.loadFromFile("../img/tx_1.png");
    opponentTilesManager.setTexture(&opponentTileTx);

    Texture texturesForUnits[9];
    texturesForUnits[0].loadFromFile("../img/unit1.png");
    texturesForUnits[1].loadFromFile("../img/unit2.png");
    texturesForUnits[2].loadFromFile("../img/unit_1.png");
    texturesForUnits[3].loadFromFile("../img/unit_2.png");
    texturesForUnits[4].loadFromFile("../img/unit_3.png");
    texturesForUnits[5].loadFromFile("../img/unit_4.png");
    texturesForUnits[6].loadFromFile("../img/unit_5.png");
    texturesForUnits[7].loadFromFile("../img/unit_6.png");
    texturesForUnits[8].loadFromFile("../img/unit_7.png");

    Unit units[9] = {
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window),
        Unit(window)};

    for (int i = 0; i < 9; ++i)
    {
        units[i].setTexture(&texturesForUnits[i]);
    }

    Texture cardTexture;
    cardTexture.loadFromFile("../img/card.png");

    Card cards[9] = {
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
        Card(window, mouse),
    };

    for (int i = 0; i < 9; ++i)
    {
        cards[i].setUnit(&units[i]);
        cards[i].setTexture(&cardTexture);
        cards[i].setColors(Color::White, Color::Magenta, Color::Green);
    }

    stack<Card *> cardsStack;

    for (int i = 0; i < 9; ++i)
    {
        cardsStack.push(&cards[i]);
    }

    Texture cardShirtTexture;
    cardShirtTexture.loadFromFile("../img/card_shirt.png");
    CardsManager cardsManager(window, cardsStack);
    cardsManager.setTilesManager(&playerTilesManager);
    cardsManager.setCardShirtTexture(&cardShirtTexture);

    Texture takeCardButtonTx;
    takeCardButtonTx.loadFromFile("../img/take_card.png");
    CommandTakeCard cmdtakecard(cardsManager);
    Button buttonTakeCard(window, mouse, &cmdtakecard);
    buttonTakeCard.setPosition(50, 30);
    buttonTakeCard.setTexture(&takeCardButtonTx);
    buttonTakeCard.setColors(Color::White, Color(20, 100, 255), Color::Magenta);

    Texture attackButtonTx;
    attackButtonTx.loadFromFile("../img/attack.png");
    Texture powerButtonTx;
    powerButtonTx.loadFromFile("../img/power.png");

    CommandAttack cmdattack(&playerTilesManager, &opponentTilesManager);
    Button attackButton(window, mouse, &cmdattack);
    attackButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80));
    attackButton.setPosition(50, 80);
    attackButton.setTexture(&attackButtonTx);

    CommandStringTest cmdtest2("CommandStringTest::execute(): Power!");
    Button powerButton(window, mouse, &cmdtest2);
    powerButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80));
    powerButton.setPosition(50, 130);
    powerButton.setTexture(&powerButtonTx);

    playerTilesManager.setButtons(&attackButton, &powerButton);

    vector<Button *> buttons = {&buttonTakeCard, &attackButton, &powerButton};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

#endif //GAME_ELEMENTS

    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/fon_1.png");
    RectangleShape backgroundRect;
    backgroundRect.setSize(Vector2f(windowWidth, windowHeight));
    backgroundRect.setTexture(&backgroundTx);
    backgroundRect.setPosition(Vector2f(0, -100));

    Texture lowerPanelTx;
    lowerPanelTx.loadFromFile("../img/low_panel.png");
    RectangleShape lowerPanelRect;
    lowerPanelRect.setSize(Vector2f(windowWidth, windowWidth / 4));
    lowerPanelRect.setTexture(&lowerPanelTx);
    lowerPanelRect.setPosition(Vector2f(0, windowHeight - windowWidth / 4 + 100));

    RectangleShape stageText;
    stageText.setSize(Vector2f(400, 100));
    stageText.setPosition((windowWidth - 400) / 2, 50);
    stageText.setFillColor(Color::White);

    Texture AvangardTx;
    AvangardTx.loadFromFile("../img/avangard.png");
    Texture FlankTx;
    FlankTx.loadFromFile("../img/flank.png");
    Texture RearTx;
    RearTx.loadFromFile("../img/rear.png");

    cout << "Other player's turn started!!" << endl;
    otherPlayersTurn(window, mouse, event, client, playerTilesManager, opponentTilesManager, cardsManager, buttonsManager, backgroundRect, lowerPanelRect);
    cout << "Other player's turn ended!!" << endl;

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            //Если двигаем мышкой
            case (Event::MouseMoved):
            {
                buttonsManager.updateFocus();
                playerTilesManager.updateFocus();
                opponentTilesManager.updateFocus();
                cardsManager.updateFocus();
                break;
            }
            //Если нажали на кнопку на мыши
            case (Event::MouseButtonPressed):
            {
                //Порядок важен!
                buttonsManager.mouseIsPressed();
                playerTilesManager.mouseIsPressed();
                opponentTilesManager.mouseIsPressed();
                cardsManager.mouseIsPressed();
                break;
            }
            //Если отпустили кнопку на мыши
            case (Event::MouseButtonReleased):
            {
                buttonsManager.mouseIsReleased();
                break;
            }

            //УДАЛИТЬ
            case (Event::KeyPressed):
            {
                if (Keyboard::isKeyPressed(Keyboard::A))
                {
                    playerTilesManager.setStage(Stage::stageAvangard);
                    stageText.setTexture(&AvangardTx);
                }
                if (Keyboard::isKeyPressed(Keyboard::B))
                {
                    playerTilesManager.setStage(Stage::stageFlank);
                    stageText.setTexture(&FlankTx);
                }
                if (Keyboard::isKeyPressed(Keyboard::C))
                {
                    playerTilesManager.setStage(Stage::stageRear);
                    stageText.setTexture(&RearTx);
                }
                playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                playerTilesManager.updateFocus();
            }
            break;

            //Закрытие окна
            case (Event::Closed):
            {
                window.close();
            }
            default:
                break;
            }
        }

        window.clear();

        window.draw(backgroundRect);
        window.draw(lowerPanelRect);
        if (playerTilesManager.getStatus() == TilesManagerStatus::statusAttackingUnit)
        {
            window.draw(stageText);
        }

        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();
    }
}

#define SERVER_CONNECTING 0

int main()
{

#if SERVER_CONNECTING == 1

    GameTcpClient client;
    if (client.connect("10.147.17.33") == false)
    {
        cout << "ERROR, CONNECTION FAILED!" << endl;
        return 0;
    }
    else
    {
        //!!!!!!!!!!!!!!!!!!!!!!!!РАСКОММЕНТИТЬ!!!!!!!!!!!!!!
        //client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::ServerAccept)
        {
            cout << "Server accepted!!" << endl;
        }
        else
        {
            cout << "Server didn't accept" << endl;
        }
    }

    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;

    if (menu(window, mouse, event, &client) == false)
    {
        cout << "Can't create lobby!" << endl;
        return 0;
    }

#endif //SERVER_CONNECTING

#if SERVER_CONNECTING == 0
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;

#endif

#if SERVER_CONNECTING == 0
    GameTcpClient *client;
#endif

    game(window, mouse, event, client);
    return 0;
}