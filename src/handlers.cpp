#include <iostream>
#include <SFML/Graphics.hpp>
#include "headers/handlers.hpp"

void handleGameEvents(sf::RenderWindow& window)
{
    while(window.isOpen())
    {
        std::cerr << "Started handler" << std::endl;
        sf::Event event;
        while(window.waitEvent(event))
        {
            std::cerr << "\tStarted while" << std::endl;
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
                }
                break;
            }
        }
    }
}

void handle()
{
    std::cerr << "b" << std::endl;
}
