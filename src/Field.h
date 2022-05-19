#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include "Tile.h"

class Field : public sf::Drawable
{
public:
    Field(sf::Vector2u size, uint32_t bombChance = 10);

    bool isGameOver() const;

    bool isSolved() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void setScale(float scale);

    void onClick(sf::Mouse::Button button, sf::Vector2i mousePosition);

private:
    bool _gameOver = false;
    bool _solved = false;

    std::vector<std::vector<Tile>> _tiles;
    sf::Vector2u _size;
    uint32_t _totalBombs = 0;

private:
    void revealNeighbours(sf::Vector2u position);
};