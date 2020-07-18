#include "maze.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Maze Demo");

    Maze maze(50);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::Closed:
                window.close();
                break;
            case sf::Event::Resized:
                window.setView(sf::View(sf::FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));
                break;
            }
        }

        maze.update();

        window.clear();

        maze.render(&window);

        window.display();
    }
}