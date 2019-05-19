#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/menu.hpp"
#include "headers/game.hpp"
#include "headers/config.hpp"

int main()
{
    if(loadConfig() == false)   //loading config file
    {
        std::cerr << "Błąd elo" << std::endl;
    }
    std::cout << CONFIG.resolution.width << " " << CONFIG.resolution.height << " " << (CONFIG.fullscreen ? "fullscreen" : "windowed") << std::endl;
    sf::RenderWindow* window;
    if(CONFIG.fullscreen)
        window = new sf::RenderWindow(sf::VideoMode(CONFIG.resolution.width, CONFIG.resolution.width), "PACMAN", sf::Style::Fullscreen);
    else
        window = new sf::RenderWindow(sf::VideoMode(CONFIG.resolution.width, CONFIG.resolution.height), "PACMAN");
    window->setFramerateLimit(CONFIG.framerate);

    switch(menu(*window))
    {
    case 0:
        delete window;
        return 0;
    case 1:
        std::cerr << "Fatal error occured. Shutting down." << std::endl;
        delete window;
        return 1;
    }
}
