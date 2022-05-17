#pragma once

#include <SFML/Graphics.hpp>

class Tile : public sf::Drawable
{
public:
    enum class Type
    {
        Empty = 0, One, Two, Three, Four, Five, Six, Seven, Eight, Bomb
    };

    Tile();

    bool empty() const;

    bool isBomb() const;

    bool isFlag() const;

    bool isHovered(sf::Vector2i mousePosition) const;

    bool isRevealed() const;

    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    void reveal();

    void setType(Type type);

    void setPosition(sf::Vector2u position);

    void setScale(float scale);

    void toggleFlag();

    void detonate();

public:
    inline static const int CELL_SIZE = 16;

private:
    Type _type;
    bool _isFlag = false;
    bool _isDetonated = false;

    sf::Sprite _sprite;
    inline static sf::Texture _texture;
    inline static bool _isInit = false;
};