#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>

#include "headers/clyde.hpp"
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/pacman.hpp"

Clyde::Clyde(MapData::Clyde clyde, float speed, std::shared_ptr<Map>& map)
    : Ghost(clyde.x, clyde.y, speed, clyde.ghostImage, clyde.ghostSprite, map)
{
    std::cerr << "Clyde created" << std::endl;
}

sf::Vector2f Clyde::getDestination(Pacman& pacman) const
{
    return pacman.getCoords();
}
