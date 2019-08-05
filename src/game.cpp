#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <string>

#include "headers/game.hpp"
#include "headers/config.hpp"
#include "headers/tile.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"
#include "headers/ghost.hpp"
#include "headers/blinky.hpp"
#include "headers/pinky.hpp"
#include "headers/inky.hpp"
#include "headers/clyde.hpp"

void processGameEvents(sf::RenderWindow& window, Pacman& player)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                return;
            case sf::Keyboard::W:
                player.turn(Direction::up);
                break;
            case sf::Keyboard::S:
                player.turn(Direction::down);
                break;
            case sf::Keyboard::A:
                player.turn(Direction::left);
                break;
            case sf::Keyboard::D:
                player.turn(Direction::right);
                break;
            case sf::Keyboard::Up:
                player.turn(Direction::up);
                break;
            case sf::Keyboard::Down:
                player.turn(Direction::down);
                break;
            case sf::Keyboard::Left:
                player.turn(Direction::left);
                break;
            case sf::Keyboard::Right:
                player.turn(Direction::right);
                break;
            }
        }
    }
}

int game(sf::RenderWindow& window)
{
    if(loadMap() == false)
        return 1;

    std::shared_ptr<Map> map = std::make_shared<Map>(CONFIG.mapData);
    Pacman player(CONFIG.mapData->pacmanX, CONFIG.mapData->pacmanY, DEFINES.PACMAN_SIZE, PACMAN_SPEED, map);

    Inky inky(CONFIG.mapData->inky, GHOST_SPEED, map);
    Blinky blinky(CONFIG.mapData->blinky, GHOST_SPEED, map);
    Pinky pinky(CONFIG.mapData->pinky, GHOST_SPEED, map);
    Clyde clyde(CONFIG.mapData->clyde, GHOST_SPEED, map);

    sf::Font font;
    font.loadFromFile("fonts/crackman.ttf");
    sf::Text score("Score: " + std::to_string(player.getScore()), font, 40);
    score.setPosition({DEFINES.MAP_MARGIN_SIDE, (DEFINES.MAP_MARGIN_TOP - score.getLocalBounds().height) / 2});
    int lifes = 3;

    while(window.isOpen())
    {
        processGameEvents(window, player);

        window.clear();

        map->draw(window);

        player.move();
        blinky.move(player);

        player.draw(window);

        blinky.draw(window);
        pinky.draw(window);
        inky.draw(window);
        clyde.draw(window);

        score.setString("Score: " + std::to_string(player.getScore()));
        window.draw(score);

        if(player.isDead())
        {
            lifes--;
            if(lifes)
            {
                player.revive();
                player.setCoords(CONFIG.mapData->pacmanX, CONFIG.mapData->pacmanY);
                player.setPosition(-1, -1);
            }
            else
            {
                std::cerr << "GAME OVER" << std::endl;
                while(window.isOpen())
                {
                    processGameEvents(window, player);
                    window.display();
                }
            }
        }

        if(map->dotsLeft() == 0)
        {
            std::cerr << "CONGRATULATIONS!" << std::endl;
            while(window.isOpen())
            {
                processGameEvents(window, player);
                window.display();
            }
        }

        window.display();
    }
    return 0;
}
