#include <SFML/Graphics.hpp>
#include "Field.h"
#include "ImGui.h"

#ifdef _MSC_VER
#include <windows.h>
int WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#else

int main()
#endif
{
    sf::Vector2u size = {10, 10};
    int scale = 4;
    sf::RenderWindow mainWindow({size.x * Tile::CELL_SIZE * scale, size.y * Tile::CELL_SIZE * scale},
                                "Minesweeper", sf::Style::Close);
    mainWindow.setVerticalSyncEnabled(true);

    float multiplier = sf::VideoMode::getDesktopMode().height / 1080.f;
    sf::RenderWindow configWindow({(uint32_t) (250 * multiplier), (uint32_t) (60 * multiplier)},
                                  "Config window", sf::Style::Titlebar);
    configWindow.setVerticalSyncEnabled(true);
    configWindow.setPosition(
            {mainWindow.getPosition().x, mainWindow.getPosition().y - (int) (configWindow.getSize().y * 1.5)});
    //    configWindow.setPosition({mainWindow.getPosition().x + (int) mainWindow.getSize().x + 100,
    //                              configWindow.getPosition().y});
    if (!ImGui::SFML::Init(configWindow))
        exit(EXIT_FAILURE);
    sf::Clock deltaClock;

    srand(time(nullptr));

    Field field(size);
    field.setScale(scale);

    int bombChance = 10;

    sf::Vector2u prevSize = size;
    int prevBombChance = bombChance;

    mainWindow.requestFocus();
    while (mainWindow.isOpen()) {
        sf::Event event = {};
        if (mainWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                mainWindow.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
                mainWindow.close();
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                field = Field(prevSize, prevBombChance);
                field.setScale(scale);
                mainWindow.setTitle("Minesweeper");
            }

            if (!field.isGameOver() && !field.isSolved()) {
                if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
                    field.onClick(sf::Mouse::Left, sf::Mouse::getPosition(mainWindow));
                if (sf::Mouse::isButtonPressed(sf::Mouse::Right))
                    field.onClick(sf::Mouse::Right, sf::Mouse::getPosition(mainWindow));
                if (field.isSolved())
                    mainWindow.setTitle("=)");
                if (field.isGameOver())
                    mainWindow.setTitle("=(");
            }
        }
        if (configWindow.pollEvent(event))
            ImGui::SFML::ProcessEvent(event);

        ImGui::SFML::Update(configWindow, deltaClock.restart());
        ImGui::Begin("Config", NULL,
                     ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar);
        ImGui::SetWindowSize(configWindow.getSize());
        ImGui::SetWindowPos({0, 0});
        ImGui::SetWindowFontScale(multiplier);
        ImGui::SliderInt2("Size", (int*) &size, 5, 30);
        ImGui::SliderInt("Bomb chance", &bombChance, 5, 90);
        if (ImGui::Button("Regenerate")) {
            auto view = mainWindow.getView();
            mainWindow.setSize({size.x * Tile::CELL_SIZE * scale, size.y * Tile::CELL_SIZE * scale});
            view.setSize(sf::Vector2f(mainWindow.getSize()));
            view.setCenter(mainWindow.getSize().x / 2, mainWindow.getSize().y / 2);
            mainWindow.setView(view);
            field = Field(size, bombChance);
            field.setScale(scale);
            mainWindow.setTitle("Minesweeper");
            prevSize = size;
            prevBombChance = bombChance;
        }
        ImGui::SameLine();
        if (ImGui::Button("Restore")) {
            size = {10, 10};
            bombChance = 10;
        }
        ImGui::End();

        configWindow.clear();
        ImGui::SFML::Render(configWindow);
        configWindow.display();

        mainWindow.clear();
        mainWindow.draw(field);
        mainWindow.display();
    }
    configWindow.close();
    ImGui::SFML::Shutdown();
}