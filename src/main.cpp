#include <iostream>
#include <vector>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
#include <boost/log/trivial.hpp>
using namespace std;
using namespace boost::log;

#include <SFML/Graphics.hpp>
using namespace sf;

#include "Button.h"
#include "Command.h"
#include "Commands/CommandAttack.h"
#include "Commands/CommandTakeCard.h"
#include "Card.h"
#include "Tile.h"
#include "Background.h"
#include "configurations.cpp"
#include "GameManager.h"
#include "Commands/CommandMakeLobby.h"

//Чтобы визуально сжать код
#define GAME_ELEMENTS 1

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
    buttonTakeCard.setColors(Color::White, Color(20, 100, 255), Color::Magenta, Color(200, 200, 200));

    Texture attackButtonTx;
    attackButtonTx.loadFromFile("../img/attack.png");
    Texture powerButtonTx;
    powerButtonTx.loadFromFile("../img/power.png");
    Texture cancelButtonTx;

    CommandAttack cmdattack(playerTilesManager, opponentTilesManager);
    Button attackButton(window, mouse, &cmdattack);
    attackButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    attackButton.setPosition(50, 80);
    attackButton.setTexture(&attackButtonTx);

    CommandStringTest cmdtest2("CommandStringTest::execute(): Power!");
    Button powerButton(window, mouse, &cmdtest2);
    powerButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    powerButton.setPosition(50, 130);
    powerButton.setTexture(&powerButtonTx);

    CommandStringTest cmdtest3("CommandStringTest::execute(): Cancel!");
    Button cancelButton(window, mouse, &cmdtest3);
    cancelButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    cancelButton.setPosition(300, 130);

    CommandStringTest cmdtest4("CommandStringTest::execute(): Remove body!");
    Button removeBodyButton(window, mouse, &cmdtest4);
    removeBodyButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    removeBodyButton.setPosition(500, 130);

    vector<Button *> buttons = {&buttonTakeCard, &attackButton, &powerButton, &cancelButton, &removeBodyButton};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/fon_1.png");

    Texture lowerPanelTx;
    lowerPanelTx.loadFromFile("../img/low_panel.png");

    Background background(window);
    background.setTextures(&backgroundTx, &lowerPanelTx);

#endif //GAME_ELEMENTS

    //BOOST_LOG_TRIVIAL(fatal) << "WTF";

    GameManager gm(window, mouse, event, client, buttonsManager, playerTilesManager, opponentTilesManager, cardsManager, background);

    gm.setAttackButton(&attackButton);
    gm.setPowerButton(&powerButton);
    gm.setTakeCardButton(&buttonTakeCard);
    gm.setCancelButton(&cancelButton);
    gm.setRemoveBodyButton(&removeBodyButton);

    gm.play();
    return 0;
}