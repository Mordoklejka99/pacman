#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "headers/pinky.hpp"
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"

Pinky::Pinky(MapData::Pinky pinky, float speed, std::shared_ptr<Map>& map)
    : Ghost(pinky.x, pinky.y, speed, pinky.ghostImage, pinky.ghostSprite, map)
{
    std::cerr << "Pinky created" << std::endl;
}

sf::Vector2f Pinky::getDestination(Pacman& pacman) const
{
    switch(pacman.getDirection())
    {
    case Direction::up:
        return pacman.getCoords() + sf::Vector2f(0, -4);
    case Direction::down:
        return pacman.getCoords() + sf::Vector2f(0, 4);
    case Direction::left:
        return pacman.getCoords() + sf::Vector2f(-4, 0);
    case Direction::right:
        return pacman.getCoords() + sf::Vector2f(4, 0);
    }
}
