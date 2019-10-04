// std c++ lib
#include <iostream>
#include <thread>

// additional lib
#include <SFML/Graphics.hpp>

// project headers
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/config.hpp"


// ctor
Map::Map(MapData& mapData)
{
    this->width = mapData.width;
    this->height = mapData.height;

    this->tiles = mapData.tiles;
}

// getters
uint Map::getWidth()
{
    return this->width;
}

uint Map::getHeight()
{
    return this->height;
}

uint Map::getDotCount()
{
    return this->dotCount;
}

// operators
Tile& Map::operator()(uint col, uint row)
{
    if(col >= this->width || row >= this->height)
        throw("Invalid indices for Map::operator()");
    return *(this->tiles[col][row]);
}

void Map::operator--()
{
    this->dotCount--;
}

// methods
void Map::draw(sf::RenderWindow& window)
{
    for(uint c = 0; c < this->width; c++)
    {
        for(uint r = 0; r < this->height; r++)
        {
            (this->tiles[c][r])->draw(window);
        }
    }
}
