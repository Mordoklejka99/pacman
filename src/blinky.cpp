// standard c++ lib
#include <iostream>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/blinky.hpp"
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/pacman.hpp"


// ctor
Blinky::Blinky(MapData& mapData, Map& map, Pacman& pacman) : Ghost(mapData, map, pacman, CONFIG.blinkyTexture)
{
    this->coords = mapData.blinky.coords;
    this->sprite->setPosition(this->coords.x, this->coords.y);
}


// methods
Position Blinky::getDestination() const
{
    return this->pacman.getPosition();
}
