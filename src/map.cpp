// std c++ lib
#include <iostream>
#include <vector>

// additional lib
#include <SFML/Graphics.hpp>

// project headers
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/config.hpp"
#include "headers/ghost.hpp"
#include "headers/pacman.hpp"


// ctor
Map::Map(MapData& mapData)
{
    this->width = mapData.width;
    this->height = mapData.height;

    this->tiles = mapData.tiles;
    this->dotCount = this->nOfDots = mapData.dotCount;
}

void Map::reconstruct(MapData& mapData)
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

    this->width = mapData.width;
    this->height = mapData.height;

    this->tiles = mapData.tiles;
    this->dotCount = this->nOfDots = mapData.dotCount;
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
void Map::restart()
{
    for(int c = 0; c < this->width; c++)
    {
        for(int r = 0; r < this->height; r++)
        {
            this->tiles[c][r]->restart();
        }
    }
}

bool Map::offTheMap(Position position) const
{
    return position.c < 0 || position.c >= this->width || position.r < 0 || position.r >= this->height;
}

std::vector<Ghost*> Map::ghostsInTile(Position position)
{
    std::vector<Ghost*> result;

    if(this->blinky->getPosition() == position)
        result.push_back(this->blinky);
    if(this->pinky->getPosition() == position)
        result.push_back(this->pinky);
    if(this->inky->getPosition() == position)
        result.push_back(this->inky);
    if(this->clyde->getPosition() == position)
        result.push_back(this->clyde);
    
    return result;
}

void Map::move()
{
    this->pacman->move();
    this->blinky->move();
    this->pinky->move();
    this->inky->move();
    this->clyde->move();
}

void Map::draw(sf::RenderWindow& window) const
{
    for(int c = 0; c < this->width; c++)
    {
        for(int r = 0; r < this->height; r++)
        {
            (this->tiles[c][r])->draw(window);
        }
    }

    this->blinky->draw(window);
    this->pinky->draw(window);
    this->inky->draw(window);
    this->clyde->draw(window);
    this->pacman->draw(window);
}
