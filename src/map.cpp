#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>

#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/config.hpp"

Map::Map(std::shared_ptr<MapData>& map)
{
    this->width = map->width;
    this->height = map->height;
    this->dotCount = map->dotCount;
    this->tiles = map->tiles;
    this->backgroundImage = map->backgroundImage;
    this->background = map->background;
}

Map::~Map()
{
    for(int c = 0; c < this->width; c++)
    {
        for(int r = 0; r < this->height; r++)
            delete this->tiles[c][r];
        delete[] this->tiles[c];
    }
    delete[] this->tiles;
}

int Map::getWidth()
{
    return this->width;
}

int Map::getHeight()
{
    return this->height;
}

int Map::dotsLeft()
{
    return this->dotCount;
}

void Map::draw(sf::RenderWindow& window)
{
    window.draw(*(this->background));
    for(int c = 0; c < this->width; c++)
        for(int r = 0; r < this->height; r++)
            this->tiles[c][r]->draw(window);
}


Tile& Map::operator()(int c, int r)
{
    if(c < this->width && r < this->height)
        return *(this->tiles[c][r]);
    else
        throw;
}

Tile& Map::operator()(sf::Vector2i pos)
{
    if(pos.x < this->width && pos.x > -1 && pos.y < this->height && pos.y > -1)
        return *(this->tiles[pos.x][pos.y]);
    else
        throw;
}

int Map::operator--(int)
{
    int val = this->dotCount;
    if(this->dotCount)
        this->dotCount--;
    return val;
}
