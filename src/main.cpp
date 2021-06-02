#include <iostream>
#include <vector>
#include <map>
#include <fstream>
#include "GameTcpClient.h"
#include "GameMsgTypes.h"
#include <boost/log/trivial.hpp>
using namespace boost::log;
using std::cout;
using std::endl;

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
#include "StageText.h"

void startGame(RenderWindow &window, Mouse &mouse, Event &event, GameTcpClient &client)
{

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

    Texture swordTx;
    swordTx.loadFromFile("../img/sword.png");
    Texture heartTx;
    heartTx.loadFromFile("../img/heart.png");

    vector<Unit> playerUnits;
    for (int i = 0; i < numberOfCards; ++i)
    {
        playerUnits.push_back(Unit(window));
        playerUnits[i].setHeartAndSwordTx(&heartTx, &swordTx);
    }

    vector<Unit> opponentUnits;
    for (int i = 0; i < numberOfCards; ++i)
    {
        opponentUnits.push_back(Unit(window));
        opponentUnits[i].setHeartAndSwordTx(&heartTx, &swordTx);
    }

    vector<Card> cards;
    for (int i = 0; i < numberOfCards; ++i)
    {
        cards.push_back(Card(window, mouse));
    }

    setData(client, playerUnits, opponentUnits, cards, texturesForUnits);
    for (int i = 0; i < playerUnits.size(); ++i)
    {
        playerUnits[i].rotateTexture();
    }

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

    Texture attackButtonTx;
    attackButtonTx.loadFromFile("../img/attack.png");
    CommandAttack cmdattack(playerTilesManager, opponentTilesManager, client);
    Button attackButton(window, mouse, cmdattack);
    attackButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    attackButton.setPosition(50, 10);
    attackButton.setSize(90, 80);
    attackButton.setTexture(&attackButtonTx);

    Texture powerButtonTx;
    powerButtonTx.loadFromFile("../img/power.png");
    CommandPower cmdpower(playerTilesManager, opponentTilesManager, client);
    Button powerButton(window, mouse, cmdpower);
    powerButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    powerButton.setPosition(155, 10);
    powerButton.setSize(90, 80);
    powerButton.setTexture(&powerButtonTx);

    Texture takeCardButtonTx;
    takeCardButtonTx.loadFromFile("../img/take_card.png");
    CommandTakeCard cmdtakecard(cardsManager, client);
    Button buttonTakeCard(window, mouse, cmdtakecard);
    buttonTakeCard.setPosition(50, 100);
    buttonTakeCard.setTexture(&takeCardButtonTx);
    buttonTakeCard.setColors(Color::White, Color(20, 100, 255), Color::Magenta, Color(200, 200, 200));

    Texture removeBodyButtonTx;
    removeBodyButtonTx.loadFromFile("../img/remove.png");
    CommandRemoveBody cmdremovebody(playerTilesManager, client);
    Button removeBodyButton(window, mouse, cmdremovebody);
    removeBodyButton.setTexture(&removeBodyButtonTx);
    removeBodyButton.setColors(Color::Cyan, Color(240, 200, 150), Color(190, 70, 80), Color(200, 200, 200));
    removeBodyButton.setPosition(50, 150);

    vector<Button *> buttons = {&buttonTakeCard, &attackButton, &powerButton, &removeBodyButton};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

    Texture backgroundTx;
    backgroundTx.loadFromFile("../img/fon_1.png");

    Texture lowerPanelTx;
    lowerPanelTx.loadFromFile("../img/low_panel.png");

    Texture infoRectTx;
    infoRectTx.loadFromFile("../img/info.png");

    Background background(window, mouse);
    background.setTextures(&backgroundTx, &lowerPanelTx, &infoRectTx);

    Texture gamestartTx;
    gamestartTx.loadFromFile("../img/gamestart.png");
    Texture avangardTx;
    avangardTx.loadFromFile("../img/avangard.png");
    Texture flankTx;
    flankTx.loadFromFile("../img/flank.png");
    Texture rearTx;
    rearTx.loadFromFile("../img/rear.png");
    Texture opponentTx;
    opponentTx.loadFromFile("../img/opponent.png");
    Texture wonTx;
    wonTx.loadFromFile("../img/won.png");
    Texture lostTx;
    lostTx.loadFromFile("../img/lost.png");

    StageText stageText(window);
    stageText.setTextures(
        &gamestartTx,
        &avangardTx,
        &flankTx,
        &rearTx,
        &opponentTx,
        &wonTx,
        &lostTx);

    //BOOST_LOG_TRIVIAL(fatal) << "WTF???";

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
                   stageText);

    gm.setSpecialButtons(attackButton, powerButton, buttonTakeCard, removeBodyButton);
    gm.play();
}

int main()
{
    setlocale(LC_ALL, "Russian");

    BOOST_LOG_TRIVIAL(info) << "main() : Starting!";

    GameTcpClient client;

    if (!connectToServer(client, "10.147.17.71"))
    {
        BOOST_LOG_TRIVIAL(fatal) << "main() : error!";
        return 0;
    }

    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Pixel Tactics");

    Mouse mouse;
    Event event;

    while (window.isOpen())
    {
        if (menu(window, mouse, event, client) == false)
        {
            BOOST_LOG_TRIVIAL(fatal) << "main() : Can't create lobby!";
            return 0;
        }
        else
        {
            BOOST_LOG_TRIVIAL(info) << "main() : Game Created!";
        }

        startGame(window, mouse, event, client);
    }

    return 0;
}
