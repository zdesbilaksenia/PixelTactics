#include "Functions.h"
#include "configurations.cpp"
#include "Button.h"
#include "Commands/CommandMakeLobby.h"
#include "Commands/CommandJoinLobby.h"

void setData(GameTcpClient &client,
             vector<Unit> &playerUnits,
             vector<Unit> &opponentUnits,
             vector<Card> &cards,
             vector<Texture> &textures)
{
    BOOST_LOG_TRIVIAL(info) << "SetData() : Starting!";
    vector<CardInfo> playerCardsInfo;
    vector<CardInfo> opponentCardsInfo;

    //Считываем имя с сервера
#if SERVER_CONNECTING == 1

    client.incoming().wait();
    auto msg = client.incoming().popFront().msg;
    if (msg.header.id == GameMsgTypes::GameDeck)
    {
        BOOST_LOG_TRIVIAL(info) << "SetData() : Trying to load playerCardsInfo!";
        msg >> playerCardsInfo;
        BOOST_LOG_TRIVIAL(info) << "SetData() : successfully loaded playerCardsInfo!";
    }

    BOOST_LOG_TRIVIAL(info) << "SetData() : loaded cards : " << endl;
    for (size_t i = 0; i < playerUnits.size(); ++i)
    {
        BOOST_LOG_TRIVIAL(info) << "||\tname: " << playerCardsInfo[i].name << " ID: " << playerCardsInfo[i].ID;
        playerUnits[i].setTexture(&textures[playerCardsInfo[i].textureId - 1]);

        string description[3];
        description[0] = playerCardsInfo[i].frontLinePower;
        description[1] = playerCardsInfo[i].middleLinePower;
        description[2] = playerCardsInfo[i].backLinePower;

        playerUnits[i].setData(playerCardsInfo[i].ID, playerCardsInfo[i].name, description, playerCardsInfo[i].HP, playerCardsInfo[i].strength);
    }

    client.incoming().wait();
    msg = client.incoming().popFront().msg;
    if (msg.header.id == GameMsgTypes::GameDeck)
    {
        BOOST_LOG_TRIVIAL(info) << "SetData() : Trying to load opponentCardsInfo!";
        msg >> opponentCardsInfo;
        BOOST_LOG_TRIVIAL(info) << "SetData() : successfully loaded opponentCardsInfo!";
    }

    BOOST_LOG_TRIVIAL(info) << "SetData() : loaded cards : " << endl;
    for (size_t i = 0; i < opponentUnits.size(); ++i)
    {
        BOOST_LOG_TRIVIAL(info) << "||\tname: " << opponentCardsInfo[i].name << " ID: " << opponentCardsInfo[i].ID;
        opponentUnits[i].setTexture(&textures[opponentCardsInfo[i].textureId - 1]);

        string description[3];
        description[0] = opponentCardsInfo[i].frontLinePower;
        description[1] = opponentCardsInfo[i].middleLinePower;
        description[2] = opponentCardsInfo[i].backLinePower;

        opponentUnits[i].setData(opponentCardsInfo[i].ID, opponentCardsInfo[i].name, description, opponentCardsInfo[i].HP, opponentCardsInfo[i].strength);
    }

#endif // SERVER_CONNECTING

#if SERVER_CONNECTING == 0

    for (size_t i = 0; i < playerUnits.size(); ++i)
    {
        playerUnits[i].setTexture(&textures[i % numberOfUnits]);
    }

#endif //SERVER_CONNECTING

    for (size_t i = 0; i < cards.size(); ++i)
    {
        cards[i].setUnit(&playerUnits[i]);
        cards[i].setColors(Color::White, Color::Magenta, Color::Green);
    }

    BOOST_LOG_TRIVIAL(info) << "SetData() : Ending!";
    return;
}

bool menu(RenderWindow &window,
          Mouse &mouse,
          Event &event,
          GameTcpClient &client)
{
    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/low_panel.png");
    RectangleShape backgroundRect;
    backgroundRect.setSize(Vector2f(windowWidth, windowHeight));
    backgroundRect.setTexture(&backgroundTx);
    backgroundRect.setPosition(Vector2f(0, -100));
    backgroundRect.setPosition(Vector2f(0, 0));

    bool lobbyWasCreated = false;
    bool joinedLobby = false;
    CommandMakeLobby cmdMakeLobby(client, lobbyWasCreated);
    //CommandStringTest cmdMakeLobby("Connecting to lobby");
    CommandJoinLobby cmdJoinLobby(client, joinedLobby);

    Texture txMakeLobby;
    txMakeLobby.loadFromFile("../img/make_lobby.png");
    Button btnMakeLobby(window, mouse, &cmdMakeLobby);
    btnMakeLobby.setTexture(&txMakeLobby);

    Button btnConnectToLobby(window, mouse, &cmdJoinLobby);

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
                cout << "menu() : lobbyWasCreated = " << lobbyWasCreated << endl;
                cout << "menu() : joinedLobby = " << joinedLobby << endl;
                if (lobbyWasCreated)
                {
                    client.setSide(0);
                    cout << "Lobby was created!!!" << endl;
                    return 1;
                }
                if (joinedLobby)
                {
                    client.setSide(1);
                    cout << "Joined lobby!!!" << endl;
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