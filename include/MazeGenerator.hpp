#ifndef MAZE_GENERATOR_HPP
#define MAZE_GENERATOR_HPP

#include "Game.hpp"

class MazeGenerator
{
public:
    MazeGenerator(sf::Vector2u size);

    const std::vector<std::vector<char>>& getMaze();
    sf::Vector2u getStartPos();

private:
    void expand(sf::Vector2u pos);

private:
    unsigned width;
    unsigned height;
    sf::Vector2u startPos;
    std::vector<std::vector<char>> maze;

    std::uniform_int_distribution<int> widthDistribution;
    std::uniform_int_distribution<int> heightDistribution;
    std::uniform_real_distribution<double> chanceDistribution;
};

#endif // MAZE_GENERATOR_HPP
