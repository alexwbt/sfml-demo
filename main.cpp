#include "maze.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Maze Demo");

    Maze maze(100);

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
            case sf::Event::MouseButtonPressed:
                const int x = (event.mouseButton.x - (int)window.getSize().x / 2) / (GRID_SIZE * maze.scale) - maze.offsetX;
                const int y = (event.mouseButton.y - (int)window.getSize().y / 2) / (GRID_SIZE * maze.scale) - maze.offsetY;
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    maze.startX = x;
                    maze.startY = y;
                }
                else
                {
                    maze.endX = x;
                    maze.endY = y;
                }
                maze.path2();
            }
        }

        maze.update();

        window.clear();

        maze.render(&window);

        window.display();
    }
}