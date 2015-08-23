#include "MazeGenerator.hpp"

MazeGenerator::MazeGenerator(sf::Vector2u size) :
    width(size.x),
    height(size.y),
    widthDistribution(0, width-3),
    heightDistribution(0, height-3),
    chanceDistribution(0, 1)
{
    // Fill the maze with uninitialized tiles
    maze.push_back(std::vector<char>(width, '#'));
    for (unsigned i = 1; i < height-1; ++i) {
        maze.push_back({});
        maze.back().push_back('#');
        for (unsigned j = 1; j < width-1; ++j) {
            maze.back().push_back('x');
        }
        maze.back().push_back('#');
    }
    maze.push_back(std::vector<char>(width, '#'));

    // Choose a start position
    if (chanceDistribution(generator) <= 0.5)
    {
        if (chanceDistribution(generator) <= 0.5)
        {
            startPos.x = 1 + widthDistribution(generator);
            startPos.y = 1;
        }
        else
        {
            startPos.x = 1 + widthDistribution(generator);
            startPos.y = height-2;
        }
    }
    else
    {
        if (chanceDistribution(generator) <= 0.5)
        {
            startPos.x = 1;
            startPos.y = 1 + heightDistribution(generator);
        }
        else
        {
            startPos.x = width-2;
            startPos.y = 1 + heightDistribution(generator);
        }
    }

    // There will be a road in all directions
    maze[startPos.y][startPos.x] = ' ';
    expand({startPos.x-1, startPos.y});
    expand({startPos.x+1, startPos.y});
    expand({startPos.x, startPos.y-1});
    expand({startPos.x, startPos.y+1});

    // The maze is not allowed to exist of more than 80% walls
    unsigned maxIterations = 25;
    unsigned wallCount = (width-2) * (height-2);
    while ((wallCount > (width-2) * (height-2) * 0.8) && (maxIterations > 0))
    {
        maxIterations--; // Prevent infinite loop if maze can't be improved anymore

        wallCount = 0;
        for (unsigned i = 0; i < height; ++i) {
            for (unsigned j = 0; j < width; ++j) {
                if (maze[i][j] != ' ')
                    wallCount++;
            }
        }

        if (wallCount < (width-2) * (height-2) * 0.8)
            break;

        for (unsigned i = 0; i < height; ++i) {
            for (unsigned j = 0; j < width; ++j) {
                if (maze[i][j] == ' ')
                    expand({j, i});
            }
        }
    }

    // All uninitialized tiles become walls
    for (unsigned i = 0; i < height; ++i) {
        for (unsigned j = 0; j < width; ++j) {
            if (maze[i][j] == 'x')
                maze[i][j] = '#';
        }
    }
}

const std::vector<std::vector<char>>& MazeGenerator::getMaze()
{
    return maze;
}

sf::Vector2u MazeGenerator::getStartPos()
{
    return startPos;
}

void MazeGenerator::expand(sf::Vector2u pos)
{
    if (maze[pos.y][pos.x] != '#')
    {
        unsigned count = 0;
        if (maze[pos.y][pos.x-1] == ' ')
            count++;
        if (maze[pos.y][pos.x+1] == ' ')
            count++;
        if (maze[pos.y-1][pos.x] == ' ')
            count++;
        if (maze[pos.y+1][pos.x] == ' ')
            count++;

        // Generation without loops
        if (count == 1)
        {
            maze[pos.y][pos.x] = ' ';

            // 95% chance to move in same direction
            if (chanceDistribution(generator) <= 0.9)
            {
                if (maze[pos.y][pos.x-1] == ' ')
                    expand({pos.x+1, pos.y});
                else if (maze[pos.y][pos.x+1] == ' ')
                    expand({pos.x-1, pos.y});
                else if (maze[pos.y-1][pos.x] == ' ')
                    expand({pos.x, pos.y+1});
                else if (maze[pos.y+1][pos.x] == ' ')
                    expand({pos.x, pos.y-1});
            }

            // 9% chance of branch
            if (chanceDistribution(generator) <= 0.09)
            {
                // Branching can happen in two directions
                if (chanceDistribution(generator) <= 0.5)
                {
                    if (maze[pos.y][pos.x-1] == ' ')
                        expand({pos.x, pos.y-1});
                    else if (maze[pos.y][pos.x+1] == ' ')
                        expand({pos.x, pos.y+1});
                    else if (maze[pos.y-1][pos.x] == ' ')
                        expand({pos.x-1, pos.y});
                    else if (maze[pos.y+1][pos.x] == ' ')
                        expand({pos.x+1, pos.y});
                }
                else
                {
                    if (maze[pos.y][pos.x-1] == ' ')
                        expand({pos.x, pos.y+1});
                    else if (maze[pos.y][pos.x+1] == ' ')
                        expand({pos.x, pos.y-1});
                    else if (maze[pos.y-1][pos.x] == ' ')
                        expand({pos.x+1, pos.y});
                    else if (maze[pos.y+1][pos.x] == ' ')
                        expand({pos.x-1, pos.y});
                }
            }
        }
        else // Allow loops (but no 4 squared next to each other)
        {
            bool allowed = true;
            if (maze[pos.y-1][pos.x-1] == ' ')
            {
                if (maze[pos.y-1][pos.x] == ' ' && maze[pos.y][pos.x-1] == ' ')
                    allowed = false;
            }
            if (maze[pos.y-1][pos.x+1] == ' ')
            {
                if (maze[pos.y-1][pos.x] == ' ' && maze[pos.y][pos.x+1] == ' ')
                    allowed = false;
            }
            if (maze[pos.y+1][pos.x-1] == ' ')
            {
                if (maze[pos.y+1][pos.x] == ' ' && maze[pos.y][pos.x-1] == ' ')
                    allowed = false;
            }
            if (maze[pos.y+1][pos.x+1] == ' ')
            {
                if (maze[pos.y+1][pos.x] == ' ' && maze[pos.y][pos.x+1] == ' ')
                    allowed = false;
            }

            if (allowed)
                maze[pos.y][pos.x] = ' ';
        }
    }
}
