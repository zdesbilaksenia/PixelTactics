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
#include "Commands/CommandPower.h"
#include "Commands/CommandTakeCard.h"
#include "Commands/CommandRemoveBody.h"
#include "Card.h"
#include "Tile.h"
#include "Background.h"
#include "configurations.cpp"
#include "GameManager.h"
#include "Functions.h"

int main()
{

    setlocale(LC_ALL, "Russian");

    BOOST_LOG_TRIVIAL(info) << "main() : Starting!";

#if SERVER_CONNECTING == 1

    GameTcpClient client;

    if (!connectToServer(client, "10.147.17.200"))
    {
        BOOST_LOG_TRIVIAL(fatal) << "main() : error!";
        return 0;
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
#endif //SERVER_CONNECTING

#if SERVER_CONNECTING == 0
    GameTcpClient client;
#endif //SERVER_CONNECTING

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

    vector<Texture> texturesForUnits(numberOfUnits);
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

    vector<Unit> playerUnits;
    for (int i = 0; i < numberOfCards; ++i)
    {
        playerUnits.push_back(Unit(window));
    }

    vector<Unit> opponentUnits;
    for (int i = 0; i < numberOfCards; ++i)
    {
        opponentUnits.push_back(Unit(window));
    }

    vector<Card> cards;
    for (int i = 0; i < numberOfCards; ++i)
    {
        cards.push_back(Card(window, mouse));
    }

    setData(client, playerUnits, opponentUnits, cards, texturesForUnits);

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
    CardsManager cardsManager(window, playerTilesManager, cardsStack);
    cardsManager.setCardShirtTexture(&cardShirtTexture);

    Texture takeCardButtonTx;
    takeCardButtonTx.loadFromFile("../img/take_card.png");
    CommandTakeCard cmdtakecard(cardsManager, client);
    Button buttonTakeCard(window, mouse, &cmdtakecard);
    buttonTakeCard.setPosition(50, 30);
    buttonTakeCard.setTexture(&takeCardButtonTx);
    buttonTakeCard.setColors(Color::White, Color(20, 100, 255), Color::Magenta, Color(200, 200, 200));

    Texture attackButtonTx;
    attackButtonTx.loadFromFile("../img/attack.png");
    Texture powerButtonTx;
    powerButtonTx.loadFromFile("../img/power.png");
    Texture cancelButtonTx;

    CommandAttack cmdattack(playerTilesManager, opponentTilesManager, client);
    Button attackButton(window, mouse, &cmdattack);
    attackButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    attackButton.setPosition(50, 80);
    attackButton.setTexture(&attackButtonTx);

    CommandPower cmdpower(playerTilesManager, opponentTilesManager, client);
    Button powerButton(window, mouse, &cmdpower);
    powerButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    powerButton.setPosition(50, 130);
    powerButton.setTexture(&powerButtonTx);

    CommandStringTest cmdtest3("CommandStringTest::execute(): Cancel!");
    Button cancelButton(window, mouse, &cmdtest3);
    cancelButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    cancelButton.setPosition(300, 130);

    CommandRemoveBody cmdremovebody(playerTilesManager, client);
    Button removeBodyButton(window, mouse, &cmdremovebody);
    removeBodyButton.setColors(Color::Cyan, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
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

    GameManager gm(window,
                   mouse,
                   event,
                   client,
                   buttonsManager,
                   playerTilesManager,
                   opponentTilesManager,
                   cardsManager,
                   background,
                   playerUnits,
                   opponentUnits,
                   texturesForUnits);

    gm.setAttackButton(attackButton);
    gm.setPowerButton(powerButton);
    gm.setTakeCardButton(buttonTakeCard);
    gm.setCancelButton(cancelButton);
    gm.setRemoveBodyButton(removeBodyButton);

    gm.play();
    return 0;
}
