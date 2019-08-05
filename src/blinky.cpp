#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "headers/blinky.hpp"
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"


Blinky::Blinky(MapData::Blinky blinky, float speed, std::shared_ptr<Map>& map)
    : Ghost(blinky.x, blinky.y, speed, blinky.ghostImage, blinky.ghostSprite, map)
{
    std::cerr << "Blinky created" << std::endl;
};

sf::Vector2f Blinky::getDestination(Pacman& pacman) const
{
    return pacman.getCoords();
}
