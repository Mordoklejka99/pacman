// standard c++ lib
#include <iostream>
#include <atomic>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/handlers.hpp"
#include "headers/pacman.hpp"

void handleGameEvents(sf::RenderWindow& window, Pacman& pacman)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        switch(event.type)
        {
        case sf::Event::Closed:
            std::cerr << "\t\tClosed" << std::endl;
            window.close();
            return;
        case sf::Event::KeyPressed:
            switch(event.key.code)
            {
            case sf::Keyboard::Escape:
                std::cerr << "\t\tEsc" << std::endl;
                window.close();
                return;
            case sf::Keyboard::Up:
                pacman.turn(Direction::up);
                break;
            case sf::Keyboard::Right:
                pacman.turn(Direction::right);
                break;
            case sf::Keyboard::Down:
                pacman.turn(Direction::down);
                break;
            case sf::Keyboard::Left:
                pacman.turn(Direction::left);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }
}

void handle()
{
    std::cerr << "b" << std::endl;
}
