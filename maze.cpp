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

    offsetX = -size / 2.0f;
    offsetY = -size / 2.0f;

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

    bool d[4] = { false, false, false, false };
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
        maze[x][y].value = --distance;
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
                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && maze[nextX][nextY].value > maze[x][y].value) {
                    x = nextX;
                    y = nextY;
                    distance = maze[nextX][nextY].value + 1;
                    break;
                }
                if (i >= 3) {
                    return;
                }
            }
        }
    }
}

void Maze::path()
{
    if (outOfBound(startX, startY) || outOfBound(endX, endY))
        return;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            maze[x][y].value = 0;
        }
    }

    const int valueDir = -1;
    int value = valueDir;
    maze[startX][startY].value = value;
    while (true) {
        for (int x = 0; x < size; x++) {
            for (int y = 0; y < size; y++) {
                if (maze[x][y].value == value) {
                    if (x == endX && y == endY) {
                        // const v = Math.abs(value);
                        // if (v > 200) {
                        //     const s = Math.abs(value) / 2 + 1;
                        //     colorG = colors([0, 255, 0], [0, 0, 255], s).concat(colors([0, 0, 255], [255, 0, 0], s));
                        // }
                        // else if (v > 1000) {
                        //     const s = Math.abs(value) / 4;
                        //     colorG = colors([0, 255, 0], [0, 255, 255], s)
                        //         .concat(colors([0, 255, 255], [0, 0, 255], s))
                        //         .concat(colors([0, 0, 255], [255, 0, 255], s))
                        //         .concat(colors([255, 0, 255], [255, 0, 0], s));
                        // }
                        // else {
                        //     colorG = colors([0, 255, 0], [255, 0, 0], v + 1);
                        // }
                        // retrace
                        while (true) {
                            if (x == startX && y == startY) {
                                break;
                            }
                            for (int i = 0; i < 4; i++) {
                                const int nextX = x + FOUR[i][0];
                                const int nextY = y + FOUR[i][1];
                                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && abs(maze[nextX][nextY].value) < abs(maze[x][y].value)) {
                                    maze[x][y].value = abs(maze[x][y].value);
                                    x = nextX;
                                    y = nextY;
                                    break;
                                }
                            }
                        }
                        return;
                    }
                    for (int i = 0; i < 4; i++) {
                        const int nextX = x + FOUR[i][0];
                        const int nextY = y + FOUR[i][1];
                        if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value == 0) {
                            maze[nextX][nextY].value = value + valueDir;
                        }
                    }
                }
            }
        }
        value += valueDir;
    }
}


void Maze::path2()
{
    if (outOfBound(startX, startY) || outOfBound(endX, endY))
        return;

    for (int x = 0; x < size; x++)
    {
        for (int y = 0; y < size; y++)
        {
            maze[x][y].value = 0;
        }
    }

    const int valueDir = -1;
    int value = valueDir;
    maze[startX][startY].value = value;
    std::vector<sf::Vector2i> heads;
    std::vector<sf::Vector2i> newHeads;
    heads.push_back(sf::Vector2i(startX, startY));
    while (true) {
        for (int h = 0; h < heads.size(); h++) {
            int x = heads[h].x;
            int y = heads[h].y;
            if (x == endX && y == endY) {
                // retrace
                while (true) {
                    if (x == startX && y == startY) {
                        break;
                    }
                    for (int i = 0; i < 4; i++) {
                        const int nextX = x + FOUR[i][0];
                        const int nextY = y + FOUR[i][1];
                        if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value != 0 && abs(maze[nextX][nextY].value) < abs(maze[x][y].value)) {
                            maze[x][y].value = abs(maze[x][y].value);
                            x = nextX;
                            y = nextY;
                            break;
                        }
                    }
                }
                return;
            }
            for (int i = 0; i < 4; i++) {
                const int nextX = x + FOUR[i][0];
                const int nextY = y + FOUR[i][1];
                if (!outOfBound(nextX, nextY) && !maze[x][y].walls[i] && maze[nextX][nextY].value == 0) {
                    maze[nextX][nextY].value = value + valueDir;
                    newHeads.push_back(sf::Vector2i(nextX, nextY));
                }
            }
        }
        heads.clear();
        heads.insert(heads.end(), newHeads.begin(), newHeads.end());
        newHeads.clear();
        value += valueDir;
    }
}

void Maze::update()
{
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) offsetY += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) offsetY -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) offsetX += 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) offsetX -= 1;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E)) scale += scale / 100;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) scale -= scale / 100;
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
    const float sx = view.x / 2.0f + (offsetX + x) * GRID_SIZE * scale;
    const float sy = view.y / 2.0f + (offsetY + y) * GRID_SIZE * scale;
    const float ex = sx + GRID_SIZE * scale;
    const float ey = sy + GRID_SIZE * scale;

    if (grid.value > 0 || (x == startX && y == startY) || (x == endX && y == endY))
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, sy)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, ey)));
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, ey)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, sy)));
    }

    if (y == 0 && grid.walls[0])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, sy)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, sy)));
    }

    if (grid.walls[1])
    {
        vertices->push_back(sf::Vertex(sf::Vector2f(sx, ey)));
        vertices->push_back(sf::Vertex(sf::Vector2f(ex, ey)));
    }

    if (x == 0 && grid.walls[2])
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
