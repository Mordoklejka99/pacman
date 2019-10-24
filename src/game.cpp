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
    if(!loadMap(mapData))
    {
        throw "Error lol";
    }

    Map map(mapData);
    Pacman pacman(mapData, map);
    Blinky blinky(mapData, map, pacman);
    Pinky pinky(mapData, map, pacman);
    Inky inky(mapData, map, pacman, blinky);
    Clyde clyde(mapData, map, pacman);

    
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

        pacman.move();
        blinky.move();
        // pinky.move();
        // inky.move();
        // clyde.move();

        map.draw(window);
        pacman.draw(window);
        blinky.draw(window);
        pinky.draw(window);
        inky.draw(window);
        clyde.draw(window);

        if(pacman.isDead())
        {
            std::cerr << "GAME OVER" << std::endl;
            while(window.isOpen())
                handleGameEvents(window, pacman);
        }

        window.display();
    }

    return 0;
}
