#pragma once

#include <iostream>

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
    float scale = 1;
    int offsetX, offsetY;
    int startX = -1, startY = -1;
    int endX = -1, endY = -1;

    Maze(int size);
    ~Maze();

    bool outOfBound(int x, int y);
    void generate(int x, int y);
    void generate2();
    void path();

    void update();
    void render(sf::RenderWindow* window);
private:
    int size;
    Grid** maze;

    void renderGrid(sf::Vector2u view, int x, int y, Grid grid, std::vector<sf::Vertex>* vertices);
};
