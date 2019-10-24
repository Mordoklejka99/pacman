// std c++ lib
#include <iostream>

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
    this->dotCount = this->nOfDots = mapData.dotCount;

    this->pinkyOut = false;
    this->inkyOut = false;
    this->clydeOut = false;
}

// dtor
Map::~Map()
{
    for(int c = 0; c < this->width; c++)
    {
        for(int r = 0; r < this->height; r++)
        {
            delete this->tiles[c][r];
        }
        delete[] this->tiles[c];
    }
    delete[] this->tiles;
}

// getters
int Map::getWidth()
{
    return this->width;
}

int Map::getHeight()
{
    return this->height;
}

int Map::getDotCount()
{
    return this->dotCount;
}

int Map::getTotalDotCount()
{
    return this->nOfDots;
}

// operators
Tile& Map::operator()(int col, int row)
{
    if(this->offTheMap(Position(col, row)))
        throw InvalidTilePositionException();
    return *(this->tiles[col][row]);
}

Tile& Map::operator()(Position position)
{
    if(this->offTheMap(position))
        throw InvalidTilePositionException();
    return *(this->tiles[position.c][position.r]);
}

void Map::operator--(int)
{
    this->dotCount--;
}

// methods
bool Map::offTheMap(Position position) const
{
    return position.c < 0 || position.c >= this->width || position.r < 0 || position.r >= this->height;
}

void Map::draw(sf::RenderWindow& window)
{
    for(int c = 0; c < this->width; c++)
    {
        for(int r = 0; r < this->height; r++)
        {
            (this->tiles[c][r])->draw(window);
        }
    }
}
