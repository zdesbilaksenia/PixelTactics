#include <iostream>
using namespace std;


#include <SFML/Graphics.hpp>
using namespace sf;

const int WindowHeight = 500;
const int WindowWidth = 500;

const int cardHeight = 200;
const int cardWidth = 100;

/*
g++ -c ../main.cpp
g++ main.o -o prog -lsfml-graphics -lsfml-window -lsfml-system
./prog
*/

class Card
{
public:
    Card(RenderWindow &_window) : window(_window){};

    void setTexture(Texture *_txCard)
    {
        rect.setTexture(_txCard);
        rect.setSize(Vector2f(cardWidth, cardHeight));
        //rect.setFillColor(Color::Cyan);
    }

    void setTextureFromFile(std::string _str)
    {
        texture.loadFromFile(_str);
        rect.setTexture(&texture);
        rect.setSize(Vector2f(cardWidth, cardHeight));
        //rect.setFillColor(Color::Cyan);
    }

    void draw()
    {
        //rect.setPosition(0, 0);
        window.draw(rect);
    }

    void setPosition(const int& _x, const int& _y)
    {
        rect.setPosition(_x, _y);
    }

private:
    Texture texture;
    RectangleShape rect;
    RenderWindow &window;
};

#define DEBUG 2

int main()
{
    RenderWindow window(sf::VideoMode(WindowHeight, WindowWidth), "SFML works!");
    //CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Red);

#if DEBUG == 1

    Texture tx;
    tx.loadFromFile("../Vampir.png");

    RectangleShape rect;
    rect.setTexture(&tx);
    rect.setSize(Vector2f(100, 100));
    rect.setPosition(0, 0);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        window.clear();

        window.draw(rect);

        window.display();
    }

    return 0;

#endif // DEBUG 1

#if DEBUG == 2

    Card Vampir(window);
    Card Tamplier(window);
    Card Okhotnik(window);

    Vampir.setTextureFromFile("../img/Vampir.png");
    Tamplier.setTextureFromFile("../img/tamplier.png");
    Okhotnik.setTextureFromFile("../img/Okhotnik.png");

    //Vampir.setTexture("../Vampir.png");
    //hero.setImage("hero.png");

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }

        Vampir.setPosition(event.mouseMove.x, event.mouseMove.y);
        //Цикл отрисовки
        window.clear();

        Vampir.draw();
        Tamplier.draw();
        Okhotnik.draw();

        window.display();
    }

    return 0;

#endif // DEBUG
}