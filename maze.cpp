#include "maze.h"

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

int FOUR[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} };

Maze::Maze(int size)
{
    this->size = size;
    maze = new Grid * [size];
    for (int x = 0; x < size; x++)
    {
        maze[x] = new Grid[size];
        for (int y = 0; y < size; y++)
        {
            maze[x][y] = { 0, new bool[4] { true, true, true, true } };
        }
    }

    offsetX = -GRID_SIZE * size / 2.0f;
    offsetY = -GRID_SIZE * size / 2.0f;

    // generate(rand() % size, rand() % size);
    generate2();
}

Maze::~Maze()
{
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            delete[] maze[x][y].walls;
        }
        delete[] maze[x];
    }
    delete[] maze;
}

bool Maze::outOfBound(int x, int y)
{
    return x < 0 || x >= size || y < 0 || y >= size;
}

int getOpDir(int dir)
{
    switch (dir)
    {
    case UP: return DOWN;
    case DOWN: return UP;
    case LEFT: return RIGHT;
    case RIGHT: return LEFT;
    }
    return -1;
}

void Maze::generate(int x, int y)
{
    maze[x][y].value = 1;

    bool d[4] = {false, false, false, false};
    while (!d[0] || !d[1] || !d[2] || !d[3])
    {
        const int dir = rand() % 4;
        if (d[dir]) continue;

        d[dir] = true;

        const int nextX = x + FOUR[dir][0];
        const int nextY = y + FOUR[dir][1];
        if (!outOfBound(nextX, nextY) && maze[nextX][nextY].value == 0)
        {
            maze[x][y].walls[dir] = false;
            maze[nextX][nextY].walls[getOpDir(dir)] = false;
            generate(nextX, nextY);
        }
    }
}

void Maze::generate2()
{
    int x = rand() % size;
    int y = rand() % size;
    int distance = 0;
    while (true) {
        maze[x][y].value = ++distance;
        bool traceBack = false;
        bool d[4] = { false, false, false, false };
        while (!traceBack) {
            const int dir = rand() % 4;
            if (!d[dir])
            {
                d[dir] = true;
                const int nextX = x + FOUR[dir][0];
                const int nextY = y + FOUR[dir][1];
                if (!outOfBound(nextX, nextY) && maze[nextX][nextY].value == 0)
                {
                    maze[x][y].walls[dir] = false;
                    maze[nextX][nextY].walls[getOpDir(dir)] = false;
                    x = nextX;
                    y = nextY;
                    break;
                }
            }
            traceBack = d[0] && d[1] && d[2] && d[3];
        }
        if (traceBack) {
            for (int i = 0; i < 4; i++) {
                const int nextX = x + FOUR[i][0];
                const int nextY = y + FOUR[i][1];
                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && maze[nextX][nextY].value < maze[x][y].value) {
                    x = nextX;
                    y = nextY;
                    distance = maze[nextX][nextY].value - 1;
                    break;
                }
                if (i >= 3) {
                    return;
                }
            }
        }
    }
}

void Maze::update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) offsetY += 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) offsetY -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) offsetX += 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) offsetX -= 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) scale += scale / 10;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) scale -= scale / 10;
}

void Maze::render(sf::RenderWindow* window)
{
    sf::Vector2u view = window->getSize();
    std::vector<sf::Vertex> vertices;
    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            renderGrid(view, x, y, maze[x][y], &vertices);
        }
    }
    window->draw(vertices.data(), vertices.size(), sf::Lines);
}

void Maze::renderGrid(sf::Vector2u view, int x, int y, Grid grid, std::vector<sf::Vertex>* vertices)
{
    const float sx = offsetX + view.x / 2.0f + x * GRID_SIZE * scale;
    const float sy = offsetY + view.y / 2.0f + y * GRID_SIZE * scale;
    const float ex = sx + GRID_SIZE * scale;
    const float ey = sy + GRID_SIZE * scale;

    if (grid.walls[0])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, sy)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, sy)));
    }

    if (grid.walls[1])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, ey)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, ey)));
    }

    if (grid.walls[2])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, sy)));
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, ey)));
    }

    if (grid.walls[3])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, sy)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, ey)));
    }
}
