#include <SFML/Graphics.hpp>
#include "Field.h"

#ifdef _MSC_VER
#include <windows.h>
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else
int main()
#endif
{
    sf::Vector2u size = {10, 10};
    int scale = 4;
    sf::RenderWindow window({size.x * Tile::CELL_SIZE * scale, size.y * Tile::CELL_SIZE * scale},
                            "Minesweeper", sf::Style::Close);
    window.setVerticalSyncEnabled(true);

    srand(time(nullptr));

    Field field(size);
    field.setScale(scale);

    while (window.isOpen()) {
        sf::Event event = {};
        if (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                window.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                field = Field(size);
                field.setScale(scale);
                window.setTitle("Minesweeper");
            }

            if (!field.isGameOver() && !field.isSolved()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    field.onClick(sf::Mouse::Left, sf::Mouse::getPosition(window));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    field.onClick(sf::Mouse::Right, sf::Mouse::getPosition(window));
                if (field.isSolved())
                    window.setTitle("=)");
                if (field.isGameOver())
                    window.setTitle("=(");
            }
        }

        window.clear();
        window.draw(field);
        window.display();
    }
}