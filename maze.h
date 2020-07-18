#pragma once

#include <SFML/Graphics.hpp>

#define GRID_SIZE 10

struct Grid
{
    int value;
    bool* walls;
};

class Maze
{
public:
    Maze(int size);
    ~Maze();

    bool outOfBound(int x, int y);
    void generate(int x, int y);
    void generate2();

    void update();
    void render(sf::RenderWindow* window);
private:
    int size;
    Grid** maze;

    int offsetX, offsetY;
    float scale = 1;

    int startX, startY;
    int endX, endY;

    void renderGrid(sf::Vector2u view, int x, int y, Grid grid, std::vector<sf::Vertex>* vertices);
};
