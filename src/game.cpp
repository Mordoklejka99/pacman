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


void restart(MapData& mapData, Map& map, Pacman& pacman, Blinky& blinky, Pinky& pinky, Inky& inky, Clyde& clyde, int level)
{
    if(!loadMap(mapData) || !loadLevel(mapData, 1))
    {
        throw "Error lol";
    }

    map.reconstruct(mapData);
    pacman.reconstruct(mapData, map);
    blinky.reconstruct(mapData, map);
    pinky.reconstruct(mapData, map);
    inky.reconstruct(mapData, map);
    clyde.reconstruct(mapData, map);
}

void restart(MapData& mapData, Map& map, Pacman& pacman, Blinky& blinky, Pinky& pinky, Inky& inky, Clyde& clyde)
{
    map.restart();
    pacman.reconstruct(mapData, map);
    blinky.reconstruct(mapData, map);
    pinky.reconstruct(mapData, map);
    inky.reconstruct(mapData, map);
    clyde.reconstruct(mapData, map);
}

void nextLevel(MapData& mapData, Map& map, Pacman& pacman, Blinky& blinky, Pinky& pinky, Inky& inky, Clyde& clyde, int level)
{
    if(!loadMap(mapData) || !loadLevel(mapData, level))
    {
        throw "Error lol";
    }

    map.reconstruct(mapData);
    pacman.reconstruct(mapData, map);
    blinky.reconstruct(mapData, map);
    pinky.reconstruct(mapData, map);
    inky.reconstruct(mapData, map);
    clyde.reconstruct(mapData, map);
}

int game()
{
    loadTextures();
    loadConfigFile();
    sf::RenderWindow window(sf::VideoMode(CONFIG.resolution.width, CONFIG.resolution.height), "PacMan");

    int level = 1;

    MapData mapData;
    if(!loadMap(mapData) || !loadLevel(mapData, level))
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

    bool restarted = false;
    bool nextLeveled = false;
    int lives = 3;

    sf::Clock clock;
    sf::Time prevTime = sf::milliseconds(0);
    while(window.isOpen())
    {
        sf::Time currTime = clock.getElapsedTime();
        sf::Time dt = currTime - prevTime;

        if(dt.asMilliseconds() < 16)
            continue;

        prevTime = currTime;

        restarted = false;
        nextLeveled = false;

        handleGameEvents(window, pacman);
        window.clear();

        map.move();
        map.draw(window);

        if(pacman.isDead())
        {
            if(--lives)
            {
                sf::Clock wait;
                while(wait.getElapsedTime().asMilliseconds() < 1000)
                    continue;
                restart(mapData, map, pacman, blinky, pinky, inky, clyde);
                continue;
            }
            std::cerr << "GAME OVER, YOU DIED!" << std::endl;
            std::cerr << "Press R to restart." << std::endl;
            std::cerr << "Pres Escape to quit." << std::endl;
            while(window.isOpen())
            {
                if(handleGameOverEvents(window) == 1)
                {
                    level = 1;
                    lives = 3;
                    restart(mapData, map, pacman, blinky, pinky, inky, clyde, level);
                    break;
                }
                window.display();
            }
        }
        if(map.getDotCount() == 0)
        {
            std::cerr << "GAME OVER, YOU WON!" << std::endl;
            std::cerr << "Press Return (Enter) to play next level." << std::endl;
            std::cerr << "Press R to restart." << std::endl;
            std::cerr << "Press Escape to quit." << std::endl;
            while(window.isOpen())
            {
                int returnCode = handleGameOverEvents(window);
                switch(returnCode)
                {
                case 1:
                    level = 1;
                    lives = 3;
                    restart(mapData, map, pacman, blinky, pinky, inky, clyde, level);
                    restarted = true;
                    break;
                case 2:
                    nextLevel(mapData, map, pacman, blinky, pinky, inky, clyde, ++level);
                    nextLeveled = true;
                    break;
                }
                window.display();
                if(restarted || nextLeveled)
                    break;
            }
        }

        window.display();
    }

    return 0;
}
