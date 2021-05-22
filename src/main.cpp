#include <iostream>
#include <vector>
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

//Чтобы визуально сжать код
#define GAME_ELEMENTS

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;

#ifdef GAME_ELEMENTS

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

    Texture skeletonTx;
    skeletonTx.loadFromFile("../img/unit1.png");
    Texture santaTx;
    santaTx.loadFromFile("../img/unit2.png");
    Texture unit_1tx;
    unit_1tx.loadFromFile("../img/unit_1.png");
    Texture unit_2tx;
    unit_2tx.loadFromFile("../img/unit_2.png");
    Texture unit_3tx;
    unit_3tx.loadFromFile("../img/unit_3.png");
    Texture unit_4tx;
    unit_4tx.loadFromFile("../img/unit_4.png");
    Texture unit_5tx;
    unit_5tx.loadFromFile("../img/unit_5.png");
    Texture unit_6tx;
    unit_6tx.loadFromFile("../img/unit_6.png");
    Texture unit_7tx;
    unit_7tx.loadFromFile("../img/unit_7.png");

    Unit skeleton(window);
    skeleton.setTexture(&skeletonTx);
    Unit santa(window);
    santa.setTexture(&santaTx);
    Unit unit_1(window);
    unit_1.setTexture(&unit_1tx);
    Unit unit_2(window);
    unit_2.setTexture(&unit_2tx);
    Unit unit_3(window);
    unit_3.setTexture(&unit_3tx);
    Unit unit_4(window);
    unit_4.setTexture(&unit_4tx);
    Unit unit_5(window);
    unit_5.setTexture(&unit_5tx);
    Unit unit_6(window);
    unit_6.setTexture(&unit_6tx);
    Unit unit_7(window);
    unit_7.setTexture(&unit_7tx);

    //CommandReleaseUnit cmdreleaseunit(playerTilesManager);

    Texture cardTexture;
    cardTexture.loadFromFile("../img/card.png");

    Card skeletonCard(window, mouse);
    skeletonCard.setUnit(&skeleton);
    skeletonCard.setTexture(&cardTexture);
    skeletonCard.setColors(Color::White, Color::Magenta, Color::Green);

    Card santaCard(window, mouse);
    santaCard.setUnit(&santa);
    santaCard.setTexture(&cardTexture);
    santaCard.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_1Card(window, mouse);
    unit_1Card.setUnit(&unit_1);
    unit_1Card.setTexture(&cardTexture);
    unit_1Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_2Card(window, mouse);
    unit_2Card.setUnit(&unit_2);
    unit_2Card.setTexture(&cardTexture);
    unit_2Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_3Card(window, mouse);
    unit_3Card.setUnit(&unit_3);
    unit_3Card.setTexture(&cardTexture);
    unit_3Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_4Card(window, mouse);
    unit_4Card.setUnit(&unit_4);
    unit_4Card.setTexture(&cardTexture);
    unit_4Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_5Card(window, mouse);
    unit_5Card.setUnit(&unit_5);
    unit_5Card.setTexture(&cardTexture);
    unit_5Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_6Card(window, mouse);
    unit_6Card.setUnit(&unit_6);
    unit_6Card.setTexture(&cardTexture);
    unit_6Card.setColors(Color::White, Color::Magenta, Color::Green);

    Card unit_7Card(window, mouse);
    unit_7Card.setUnit(&unit_7);
    unit_7Card.setTexture(&cardTexture);
    unit_7Card.setColors(Color::White, Color::Magenta, Color::Green);

    stack<Card *> cards;
    cards.push(&santaCard);
    cards.push(&skeletonCard);
    cards.push(&unit_1Card);
    cards.push(&unit_2Card);
    cards.push(&unit_3Card);
    cards.push(&unit_4Card);
    cards.push(&unit_5Card);
    cards.push(&unit_6Card);
    cards.push(&unit_7Card);

    Texture cardShirtTexture;
    cardShirtTexture.loadFromFile("../img/card_shirt.png");
    CardsManager cardsManager(window, cards);
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

    CommandStringTest cmdtest2("CommandStringTest::execute(): Magic!");
    Button powerButton(window, mouse, &cmdtest2);
    powerButton.setColors(Color::White, Color(240, 200, 150), Color(190, 70, 80));
    powerButton.setPosition(50, 130);
    powerButton.setTexture(&powerButtonTx);

    playerTilesManager.setButtons(&attackButton, &powerButton);

    vector<Button *> buttons = {&buttonTakeCard, &attackButton, &powerButton};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

#endif //GAME_ELEMENTS

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
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }
                if (Keyboard::isKeyPressed(Keyboard::B))
                {
                    playerTilesManager.setStage(Stage::stageFlank);
                    stageText.setTexture(&FlankTx);
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }
                if (Keyboard::isKeyPressed(Keyboard::C))
                {
                    playerTilesManager.setStage(Stage::stageRear);
                    stageText.setTexture(&RearTx);
                    playerTilesManager.setStatus(TilesManagerStatus::statusAttackingUnit);
                    playerTilesManager.updateFocus();
                }
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

    return 0;
}
