#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "headers/inky.hpp"
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"

Inky::Inky(MapData::Inky inky, float speed, std::shared_ptr<Map>& map)
    : Ghost(inky.x, inky.y, speed, inky.ghostImage, inky.ghostSprite, map)
{
    std::cerr << "Inky created" << std::endl;
}

sf::Vector2f Inky::getDestination(Pacman& pacman) const
{
    return pacman.getCoords();
}
