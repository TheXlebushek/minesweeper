#include "Tile.h"

Tile::Tile(uint32_t bombChance)
{
    int chance = rand() % 100;
    if (chance < 100 - bombChance)
        _type = Type::Empty;
    else
        _type = Type::Bomb;

    if (!_isInit)
        if (_texture.loadFromFile("resources/tiles.png"))
            _isInit = true;
        else
            exit(EXIT_FAILURE);

    _sprite.setTexture(_texture);
    _sprite.setTextureRect({0, 0, CELL_SIZE, CELL_SIZE});
}

bool Tile::empty() const
{
    return _type == Type::Empty;
}

bool Tile::isBomb() const
{
    return _type == Type::Bomb;
}

bool Tile::isFlag() const
{
    return _isFlag;
}

bool Tile::isHovered(sf::Vector2i mousePosition) const
{
    return _sprite.getGlobalBounds().contains((sf::Vector2f) mousePosition);
}

bool Tile::isRevealed() const
{
    const int flag = 12;
    return !(_sprite.getTextureRect().left == 0 || _sprite.getTextureRect().left == flag * CELL_SIZE);
}

void Tile::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(_sprite, states);
}

void Tile::reveal()
{
    _sprite.setTextureRect({((int) _type + 1 + _isDetonated) * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE});
}

void Tile::setType(Tile::Type type)
{
    _type = type;
}

void Tile::setPosition(sf::Vector2u position)
{
    _sprite.setPosition((sf::Vector2f) position);
}

void Tile::setScale(float scale)
{
    _sprite.setScale(scale, scale);
}

void Tile::toggleFlag()
{
    const int flag = 12;
    _isFlag = !_isFlag;
    _sprite.setTextureRect({flag * _isFlag * CELL_SIZE, 0, CELL_SIZE, CELL_SIZE});
}

void Tile::detonate()
{
    _isDetonated = true;
    reveal();
}