#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
#include <boost/log/trivial.hpp>
using namespace boost::log;
using namespace std;

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

void setData(GameTcpClient &client, vector<Unit> &units, vector<Card> &cards, Texture textures[numberOfUnits])
{
    BOOST_LOG_TRIVIAL(info) << "SetData() : Starting!";
    /*
    enum unitNames
    {
        knight,
        witch,
        alchemist,
        berserk,
        fighter,
        vampire,
        priestess,
        dragonmage,
        animaltrapper,
        oracle,
        paladin
    };

    map<string, unitNames> names;
    names["Рыцарь"] = knight;
    names["Ведьма"] = witch;
    names["Алхимик"] = alchemist;
    names["Берсерк"] = berserk;
    names["Боец"] = fighter;
    names["Вампир"] = vampire;
    names["Жрица"] = priestess;
    names["Маг-дракон"] = dragonmage;
    names["Зверолов"] = animaltrapper;
    names["Оракул"] = oracle;
    names["Паладин"] = paladin;
    */

    vector<CardInfo> cardsInfo;

    //Считываем имя с сервера
#if SERVER_CONNECTING == 1

    client.incoming().wait();
    auto msg = client.incoming().popFront().msg;
    //Тип сообщения должен быть GameMsgTypes::GameHeroStats
    if (msg.header.id == GameMsgTypes::GameDeck)
    {
        BOOST_LOG_TRIVIAL(info) << "SetData() : Trying to load cardsInfo!";
        msg >> cardsInfo;
        BOOST_LOG_TRIVIAL(info) << "SetData() : successfully loaded cardsInfo!";
    }

    for (size_t i = 0; i < units.size(); ++i)
    {
        cout << "name " << cardsInfo[i].name << " ID = " << cardsInfo[i].ID << endl;
        units[i].setTexture(&textures[cardsInfo[i].ID - 1]);

        string description[3];
        description[0] = cardsInfo[i].frontLinePower;
        description[1] = cardsInfo[i].middleLinePower;
        description[2] = cardsInfo[i].backLinePower;

        units[i].setData(cardsInfo[i].ID, cardsInfo[i].name, description, cardsInfo[i].HP, cardsInfo[i].strength);
    }

#endif // SERVER_CONNECTING

#if SERVER_CONNECTING == 0

    for (size_t i = 0; i < units.size(); ++i)
    {
        units[i].setTexture(&textures[i % numberOfUnits]);
    }

#endif //SERVER_CONNECTING

    for (size_t i = 0; i < cards.size(); ++i)
    {
        cards[i].setUnit(&units[i]);
        cards[i].setColors(Color::White, Color::Magenta, Color::Green);
    }

    BOOST_LOG_TRIVIAL(info) << "SetData() : Ending!";
    return;
}

class CommandJoinLobby : public Command
{
public:
    CommandJoinLobby(GameTcpClient &_client) : client(_client)
    {
    }
    void execute() override
    {
        client.joinLobby(1);
        if (client.isConnected())
        {
            BOOST_LOG_TRIVIAL(info) << "CommandJoinLobby::execute() : Lobby waiting for player";
            client.incoming().wait();
            auto msg = client.incoming().popFront().msg;
            if (msg.header.id == GameMsgTypes::LobbyGameStart)
            {
                BOOST_LOG_TRIVIAL(info) << "CommandJoinLobby::execute() : Lobby Game started!";
                return;
            }
            else
            {
                BOOST_LOG_TRIVIAL(error) << "CommandJoinLobby::execute() : Didn't recieve LobbyWaitingForPlayer";
            }
        }
    }

    ~CommandJoinLobby() {}

private:
    GameTcpClient &client;
};

bool menu(RenderWindow &window,
          Mouse &mouse,
          Event &event,
          GameTcpClient& client)
{
    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/low_panel.png");
    RectangleShape backgroundRect;
    backgroundRect.setSize(Vector2f(windowWidth, windowHeight));
    backgroundRect.setTexture(&backgroundTx);
    backgroundRect.setPosition(Vector2f(0, -100));
    backgroundRect.setPosition(Vector2f(0, 0));

    bool lobbyWasCreated = false;
    CommandMakeLobby cmdMakeLobby(client, lobbyWasCreated);
    //CommandStringTest cmdMakeLobby("Connecting to lobby");
    CommandJoinLobby cmdJoinLobby(client);

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

int main()
{

    BOOST_LOG_TRIVIAL(info) << "main() : Starting!";

#if SERVER_CONNECTING == 1

    GameTcpClient client;
    if (client.connect("10.147.17.200") == false)
    {
        BOOST_LOG_TRIVIAL(fatal) << "main() : Connection failed!";
        return 0;
    }
    else
    {
        //!!!!!!!!!!!!!!!!!!!!!!!!РАСКОММЕНТИТЬ!!!!!!!!!!!!!!
        client.incoming().wait();
        auto msg = client.incoming().popFront().msg;
        if (msg.header.id == GameMsgTypes::ServerAccept)
        {
            BOOST_LOG_TRIVIAL(info) << "main() : Server accepted!";
        }
        else
        {
            BOOST_LOG_TRIVIAL(fatal) << "main() : Server didn't accept!";
            return 0;
        }
    }

    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;

    if (menu(window, mouse, event, client) == false)
    {
        BOOST_LOG_TRIVIAL(fatal) << "main() : Can't create lobby!";
        return 0;
    }
    else
    {
        BOOST_LOG_TRIVIAL(info) << "main() : Game Created!";
    }

#endif //SERVER_CONNECTING

#if SERVER_CONNECTING == 0
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;

#endif

#if SERVER_CONNECTING == 0
    GameTcpClient client;
#endif

#if GAME_ELEMENTS == 1

    TilesManager playerTilesManager(window, mouse, Side::sidePlayer, client);
    TilesManager opponentTilesManager(window, mouse, Side::sideOpponent, client);

    //Текстура для союзных тайлов
    Texture playerTileTx;
    playerTileTx.loadFromFile("../img/tx_2.png");
    playerTilesManager.setTexture(&playerTileTx);
    //Текстура для тайлов соперника
    Texture opponentTileTx;
    opponentTileTx.loadFromFile("../img/tx_1.png");
    opponentTilesManager.setTexture(&opponentTileTx);

    Texture texturesForUnits[numberOfUnits];
    texturesForUnits[0].loadFromFile("../img/units/knight.png");
    texturesForUnits[1].loadFromFile("../img/units/witch.png");
    texturesForUnits[2].loadFromFile("../img/units/alchemist.png");
    texturesForUnits[3].loadFromFile("../img/units/berserk.png");
    texturesForUnits[4].loadFromFile("../img/units/fighter.png");
    texturesForUnits[5].loadFromFile("../img/units/vampire.png");
    texturesForUnits[6].loadFromFile("../img/units/priestess.png");
    texturesForUnits[7].loadFromFile("../img/units/dragonmage.png");
    texturesForUnits[8].loadFromFile("../img/units/animaltrapper.png");
    texturesForUnits[9].loadFromFile("../img/units/oracle.png");
    texturesForUnits[10].loadFromFile("../img/units/paladin.png");

    vector<Unit> units;
    for (int i = 0; i < numberOfCards; ++i)
    {
        units.push_back(Unit(window));
    }

    vector<Card> cards;
    for (int i = 0; i < numberOfCards; ++i)
    {
        cards.push_back(Card(window, mouse));
    }

    setData(client, units, cards, texturesForUnits);

    Texture cardTexture;
    cardTexture.loadFromFile("../img/card.png");
    for (int i = 0; i < numberOfCards; ++i)
    {
        cards[i].setTexture(&cardTexture);
    }

    stack<Card *> cardsStack;

    for (int i = 0; i < numberOfCards; ++i)
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

    gm.setAttackButton(attackButton);
    gm.setPowerButton(powerButton);
    gm.setTakeCardButton(buttonTakeCard);
    gm.setCancelButton(cancelButton);
    gm.setRemoveBodyButton(removeBodyButton);

    gm.play();
    return 0;
}
