// standard c++ lib
#include <iostream>
#include <thread>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/game.hpp"
#include "headers/handlers.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"


int game()
{
    std::thread loadTexturesThread(loadTextures);
    loadConfigFile();
    loadTexturesThread.join();
    sf::RenderWindow window(sf::VideoMode(CONFIG.resolution.width, CONFIG.resolution.height), "PacMan");

    MapData mapData;
    if(!loadMap(mapData))
    {
        throw "Error lol";
    }

    std::thread handlersThread(handleGameEvents, std::ref(window));
    Map map(mapData);
    
    while(window.isOpen())
    {
        window.clear();

        map.draw(window);

        window.display();
    }

    handlersThread.join();
    return 0;
}
