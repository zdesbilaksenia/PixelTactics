#include <iostream>
#include <vector>
using namespace std;

#include <SFML/Graphics.hpp>
using namespace sf;

/*
const int windowHeight = 720;
const int windowWidth = 1080;
const int buttonWidth = 200;
const int buttonHeight = 40;
const int tileWidth = 120;
const int tileHeight = 80;
const int cardWidth = 150;
const int cardHeight = 250;
*/

#include "Button.h"
#include "Command.h"
#include "Commands/CommandAttackTile.h"
//#include "Commands/CommandSetUnit.h"
#include "Commands/CommandReleaseUnit.h"
#include "Card.h"
#include "configurations.cpp"

/*
1 - кнопки
2 - тайлы
3 - юниты
4 - карты
*/
#define DEBUG 2

#if DEBUG == 1

int main()
{
    RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "SFML works!");

    Mouse mouse;
    Event event;
    CommandTest cmd;
    CommandStringTest cmdstr("Text for CommandStringTest!");
    CommandStringTest cmdstr_2("Second text!");

    Button testButton1(window, mouse, &cmd);
    testButton1.setSize(buttonWidth, buttonHeight);
    testButton1.setPosition(100, 300);
    testButton1.setColors(Color::Red, Color::Green, Color::Yellow);

    Button testButton2(window, mouse, &cmdstr);
    testButton2.setSize(buttonWidth, buttonHeight);
    testButton2.setPosition(400, 300);
    testButton2.setColors(Color::Red, Color::Green, Color::Yellow);

    Button testButton3(window, mouse, &cmdstr_2);
    testButton3.setSize(buttonWidth, buttonHeight);
    testButton3.setPosition(700, 300);
    testButton3.setColors(Color::Red, Color::Green, Color::Yellow);

    //Без указателей не получалось почему-то
    vector<Button *> buttons = {&testButton1, &testButton2, &testButton3};

    while (window.isOpen())
    {
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            //Если двигаем мышкой
            case (Event::MouseMoved):
            {
                // Смотрим, какие кнопки в фокусе, что бы их соответствующим образом отрисовывать
                for (auto button : buttons)
                {
                    button->updateFocus();
                }
                break;
            }
            //Если нажали на кнопку на мыши
            case (Event::MouseButtonPressed):
            {
                // Смотрим, на какую кнопку нажали
                for (auto button : buttons)
                {
                    if (button->hasFocus())
                    {
                        button->click();
                    }
                }
                break;
            }
            //Если отпустили кнопку на мыши
            case (Event::MouseButtonReleased):
            {
                for (auto button : buttons)
                {
                    if (button->hasFocus())
                    {
                        button->release();
                    }
                }
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

        for (auto button : buttons)
        {
            button->draw();
        }

        window.display();
    }
}

#endif

#if DEBUG == 2

#include "Tile.h"

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

    //Текстура для союзных тайлов
    Texture playerTileTx;
    playerTileTx.loadFromFile("../img/tx_2.png");
    //Текстура для тайлов соперника
    Texture opponentTileTx;
    opponentTileTx.loadFromFile("../img/tx_1.png");

    vector<Tile *> playerTiles;
    for (int i = 0; i < 9; ++i)
    {
        playerTiles.push_back(new Tile(window, mouse));
        playerTiles[i]->setSize(tileWidth, tileHeight);
        playerTiles[i]->setPosition(150 * (i % 3 + 1) - 80, 100 * (i / 3 + 1) + 100);
        playerTiles[i]->setTexture(&playerTileTx);
        playerTiles[i]->setStatus(statusIsEmpty);

        //Случайным образом выбрал тайлы, с которыми будет работать команда, вычисляющая, какие поля можно атаковать.
        if (i % 3 + 1 == i / 3 + 1)
        {
            playerTiles[i]->setStatus(statusHasUnit);
        }
    }

    playerTiles[2]->setStatus(statusHasDeadBody);

    vector<Tile *> opponentTiles;
    for (int i = 0; i < 9; ++i)
    {
        opponentTiles.push_back(new Tile(window, mouse));
        opponentTiles[i]->setSize(tileWidth, tileHeight);
        //Числа захардкожены!!!
        opponentTiles[i]->setPosition(150 * (i % 3 + 1) + windowWidth - 520 - tileWidth, 100 * (i / 3 + 1) + 100);
        opponentTiles[i]->setTexture(&opponentTileTx);
        opponentTiles[i]->setFillColor(Color::White);

        //Случайным образом выбрал тайлы, с которыми будет работать команда, вычисляющая, какие поля можно атаковать.
        if (i % 3 == i / 3 + 2)
        {
            opponentTiles[i]->setStatus(statusHasUnit);
        }
    }

    TilesManager playerTilesManager;
    playerTilesManager.setTiles(playerTiles);

    TilesManager opponentTilesManager;
    opponentTilesManager.setTiles(opponentTiles);

    Texture skeletonTx;
    skeletonTx.loadFromFile("../img/unit1.png");
    Unit skeleton(window);
    skeleton.setTexture(&skeletonTx);

    CommandReleaseUnit cmdreleaseunit(&skeleton, playerTilesManager);

    /*
    CommandSetUnit cmdsetunit(&skeleton, playerTilesManager);
    Texture buttonSetUnitTx;
    buttonSetUnitTx.loadFromFile("../img/release.png");
    Button buttonSetUnit(window, mouse, &cmdsetunit);
    buttonSetUnit.setTexture(&buttonSetUnitTx);
    buttonSetUnit.setSize(buttonWidth, buttonHeight);
    buttonSetUnit.setPosition(800, 665);
    buttonSetUnit.setColors(Color::White, Color::Green, Color::Magenta);

    Texture button1Tx;
    button1Tx.loadFromFile("../img/release1.png");
    Button buttonTest(window, mouse, &cmdstr);
    buttonTest.setTexture(&button1Tx);
    buttonTest.setSize(buttonWidth, buttonHeight);
    buttonTest.setPosition(800, 615);
    buttonTest.setColors(Color::Magenta, Color::Green, Color::Yellow);

    Texture santaTx;
    santaTx.loadFromFile("../img/unit2.png");
    Unit santa(window);
    santa.setTexture(&santaTx);
    CommandSetUnit cmdsetunit_2(&santa, playerTilesManager);
    Texture buttonSetUnitTx_2;
    buttonSetUnitTx_2.loadFromFile("../img/release.png");
    Button buttonSetUnit_2(window, mouse, &cmdsetunit_2);
    buttonSetUnit_2.setTexture(&buttonSetUnitTx);
    buttonSetUnit_2.setSize(buttonWidth, buttonHeight);
    buttonSetUnit_2.setPosition(500, 665);
    buttonSetUnit_2.setColors(Color::Cyan, Color::Green, Color::Magenta);
    */

    
    //Без указателей не получалось почему-то
    vector<Button *> buttons = {};
    ButtonsManager buttonsManager;
    buttonsManager.setButtons(buttons);

    Texture cardTexture;
    cardTexture.loadFromFile("../img/card.png");
    Card skeletonCard(window, mouse, &cmdreleaseunit);
    skeletonCard.setUnit(&skeleton);
    skeletonCard.setTexture(&cardTexture);
    skeletonCard.setSize(cardWidth, cardHeight);
    skeletonCard.setPosition(300, 500);
    skeletonCard.setColors(Color::White, Color::Magenta, Color::Green);
    vector<Card *> cards = {&skeletonCard};

    CardsManager cardsManager;
    cardsManager.setCards(cards);
    cardsManager.setTilesManager(&playerTilesManager);

#endif //GAME_ELEMENTS

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
                cardsManager.updateFocus();
                break;
            }
            //Если нажали на кнопку на мыши
            case (Event::MouseButtonPressed):
            {
                buttonsManager.mouseIsPressed();
                playerTilesManager.mouseIsPressed();
                cardsManager.mouseIsPressed();
                break;
            }
            //Если отпустили кнопку на мыши
            case (Event::MouseButtonReleased):
            {
                buttonsManager.mouseIsReleased();
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
        window.draw(lowerPanelRect);

        buttonsManager.draw();
        playerTilesManager.draw();
        opponentTilesManager.draw();
        cardsManager.draw();

        window.display();
    }
}

#endif