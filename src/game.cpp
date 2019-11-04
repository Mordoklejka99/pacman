// standard c++ lib
#include <iostream>

// additional libs
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// project headers
#include "headers/game.hpp"
#include "headers/handlers.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/pacman.hpp"
#include "headers/ghost.hpp"


int game()
{
    loadTextures();
    loadConfigFile();
    sf::RenderWindow window(sf::VideoMode(CONFIG.resolution.width, CONFIG.resolution.height), "PacMan");

    MapData mapData;
    if(!loadMap(mapData) || !loadLevel(mapData, 1))
    {
        throw "Error lol";
    }

    Map map(mapData);
    Pacman pacman(mapData, map);
    Blinky blinky(mapData, map);
    Pinky pinky(mapData, map);
    Inky inky(mapData, map);
    Clyde clyde(mapData, map);

    map.pacman = &pacman;
    map.blinky = &blinky;
    map.pinky = &pinky;
    map.inky = &inky;
    map.clyde = &clyde;

    
    sf::Clock clock;
    sf::Time prevTime = sf::milliseconds(0);
    while(window.isOpen())
    {
        sf::Time currTime = clock.getElapsedTime();
        sf::Time dt = currTime - prevTime;

        if(dt.asMilliseconds() < 16)
            continue;

        prevTime = currTime;

        handleGameEvents(window, pacman);
        window.clear();

        map.move();
        map.draw(window);

        if(pacman.isDead())
        {
            std::cerr << "GAME OVER, YOU DIED!" << std::endl;
            while(window.isOpen())
            {
                handleGameEvents(window, pacman);
                window.display();
            }
        }
        if(map.getDotCount() == 0)
        {
            std::cerr << "GAME OVER, YOU WON!" << std::endl;
            while(window.isOpen())
            {
                handleGameEvents(window, pacman);
                window.display();
            }
        }

        window.display();
    }

    return 0;
}
