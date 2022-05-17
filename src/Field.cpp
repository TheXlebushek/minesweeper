#include "Field.h"

Field::Field(sf::Vector2u size)
{
    _size = size;
    for (int i = 0; i < _size.x; ++i) {
        _tiles.emplace_back(std::vector<Tile>());
        for (int j = 0; j < _size.y; ++j) {
            _tiles[i].emplace_back(Tile());
            _totalBombs += _tiles[i][j].isBomb();
        }
    }

    for (int i = 0; i < _size.x; ++i)
        for (int j = 0; j < _size.y; ++j) {
            if (_tiles[i][j].isBomb())
                continue;
            int amountOfBombs = 0;
            for (int a = -1; a <= 1; ++a)
                for (int b = -1; b <= 1; ++b) {
                    if (!(a || b))
                        continue;
                    if (i + a >= 0 && i + a < _size.x)
                        if (j + b >= 0 && j + b < _size.y)
                            if (_tiles[i + a][j + b].isBomb())
                                ++amountOfBombs;
                }
            _tiles[i][j].setType(Tile::Type(amountOfBombs));
        }
}

bool Field::isGameOver() const
{
    return _gameOver;
}

bool Field::isSolved() const
{
    return _solved;
}

void Field::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    for (auto& vec: _tiles)
        for (auto& tile: vec)
            target.draw(tile, states);
}

void Field::setScale(float scale)
{
    for (int i = 0; i < _size.x; ++i)
        for (int j = 0; j < _size.y; ++j) {
            _tiles[i][j].setPosition(sf::Vector2u(i * scale * Tile::CELL_SIZE, j * scale * Tile::CELL_SIZE));
            _tiles[i][j].setScale(scale);
        }
}

void Field::onClick(sf::Mouse::Button button, sf::Vector2i mousePosition)
{
    if (button == sf::Mouse::Left)
        for (int i = 0; i < _size.x; ++i)
            for (int j = 0; j < _size.y; ++j) {
                if (_gameOver)
                    break;
                if (_tiles[i][j].isHovered(mousePosition)) {
                    if (_tiles[i][j].isFlag())
                        break;
                    if (_tiles[i][j].isBomb()) {
                        _tiles[i][j].detonate();
                        _gameOver = true;
                    }
                    else if (_tiles[i][j].empty()) {
                        _tiles[i][j].reveal();
                        revealNeighbours(sf::Vector2u(i, j));
                    }
                    else
                        _tiles[i][j].reveal();
                }
            }

    if (button == sf::Mouse::Right)
        for (int i = 0; i < _size.x; ++i)
            for (int j = 0; j < _size.y; ++j) {
                if (_gameOver)
                    break;
                if (_tiles[i][j].isHovered(mousePosition))
                    if (!_tiles[i][j].isRevealed())
                        _tiles[i][j].toggleFlag();
            }

    if (_gameOver)
        for (auto& vec: _tiles)
            for (auto& tile: vec)
                tile.reveal();

    for (auto& vec: _tiles)
        for (auto& tile: vec)
            if (!tile.isRevealed() && !tile.isBomb())
                return;
    _solved = true;
}

void Field::revealNeighbours(sf::Vector2u position)
{
    for (int a = -1; a <= 1; ++a)
        for (int b = -1; b <= 1; ++b) {
            if (a == b)
                continue;
            if (position.x + a >= 0 && position.x + a < _size.x)
                if (position.y + b >= 0 && position.y + b < _size.y) {
                    if (_tiles[position.x + a][position.y + b].empty())
                        if (!_tiles[position.x + a][position.y + b].isRevealed())
                            if (!_tiles[position.x + a][position.y + b].isFlag()) {
                                _tiles[position.x + a][position.y + b].reveal();
                                revealNeighbours({position.x + a, position.y + b});
                            }
                    if (!_tiles[position.x + a][position.y + b].isBomb())
                        if (!_tiles[position.x + a][position.y + b].isRevealed())
                            if (!_tiles[position.x + a][position.y + b].isFlag())
                                _tiles[position.x + a][position.y + b].reveal();
                }
        }
}