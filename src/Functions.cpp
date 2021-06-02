#include "Functions.h"
#include "configurations.cpp"
#include "Button.h"
#include "Commands/CommandConnectToLobby.h"

bool connectToServer(GameTcpClient &client, const string &host)
{
    BOOST_LOG_TRIVIAL(info) << "connectToServer() : trying to connect to " << host;
    if (client.connect(host) == false)
    {
        BOOST_LOG_TRIVIAL(fatal) << "connectToServer() : Connection failed!";
        return false;
    }
    else
    {
        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::ServerAccept)
        {
            BOOST_LOG_TRIVIAL(info) << "connectToServer() : Server accepted!";
            return true;
        }
        else
        {
            BOOST_LOG_TRIVIAL(fatal) << "connectToServer() : Server didn't accept!";
            return false;
        }
    }
}

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

        playerUnits[i].setFont("../src/Pixel.ttf");
        playerUnits[i].setTextHP(playerCardsInfo[i].HP);
        playerUnits[i].setTextAttack(playerCardsInfo[i].strength);
        playerUnits[i].rotateTexture();

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

        opponentUnits[i].setFont("../src/Pixel.ttf");
        opponentUnits[i].setTextHP(opponentCardsInfo[i].HP);
        opponentUnits[i].setTextAttack(opponentCardsInfo[i].strength);

        string description[3];
        description[0] = opponentCardsInfo[i].frontLinePower;
        description[1] = opponentCardsInfo[i].middleLinePower;
        description[2] = opponentCardsInfo[i].backLinePower;


        opponentUnits[i].setData(opponentCardsInfo[i].ID, opponentCardsInfo[i].name, description, opponentCardsInfo[i].HP, opponentCardsInfo[i].strength);
    }

    for (size_t i = 0; i < cards.size(); ++i)
    {
        cards[i].setUnit(playerUnits[i]);
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
    backgroundRect.setPosition(Vector2f(0, 0));

    Texture tacticsTx;
    tacticsTx.loadFromFile("../img/tactics.png");
    RectangleShape tacticsRect;
    tacticsRect.setSize(Vector2f(700, 320));
    tacticsRect.setTexture(&tacticsTx);
    tacticsRect.setPosition(Vector2f((windowWidth - 700)/2, 100));

    bool lobbyWasCreated = false;
    CommandConnectToLobby cmdConnectToLobby(client, lobbyWasCreated);

    Texture txMakeLobby;
    txMakeLobby.loadFromFile("../img/play.png");
    Button btnConnectToLobby(window, mouse, cmdConnectToLobby);
    btnConnectToLobby.setTexture(&txMakeLobby);
    btnConnectToLobby.setColors(Color::White, Color(140, 20, 230), Color::Green);
    btnConnectToLobby.setSize(playButtonWidth, playButtonHeight);
    btnConnectToLobby.setPosition((windowWidth - playButtonWidth) / 2, 600);

    vector<Button *> buttons = {&btnConnectToLobby};
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
                    BOOST_LOG_TRIVIAL(info) << "menu() : Lobby was created!";
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
        window.draw(tacticsRect);
        buttonsManager.draw();

        window.display();
    }
    return 0;
}