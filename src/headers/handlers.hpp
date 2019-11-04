#ifndef HANDLERS_HPP
#define HANDLERS_HPP

namespace sf
{
    class RenderWindow;
};

class Pacman;

void handleMenuEvents(sf::RenderWindow& window);
void handleGameEvents(sf::RenderWindow& window, Pacman& pacman);
int handleGameOverEvents(sf::RenderWindow& window);

#endif
