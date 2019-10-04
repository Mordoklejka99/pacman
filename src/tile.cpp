// standard c++ lib
#include <iostream>
#include <memory>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/tile.hpp"
#include "headers/config.hpp"


// ctors
Tile::Tile(Position position, TileContents content)
{
    this->position = position;
    this->coords.x = DEFINES.SIDE_MARGIN + float(this->position.c) * DEFINES.TILE_SIZE;
    this->coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + float(this->position.r) * DEFINES.TILE_SIZE;
    this->content = content;

    this->sprite = nullptr;
}

Tile::Tile(Position position, TileContents content, sf::Texture* texture)
{
    this->position = position;
    this->coords.x = DEFINES.SIDE_MARGIN + this->position.c * DEFINES.TILE_SIZE;
    this->coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + this->position.r * DEFINES.TILE_SIZE;
    this->content = content;

    this->sprite = new sf::Sprite(*texture);
    this->sprite->setScale(float(DEFINES.TILE_SIZE) / texture->getSize().x, float(DEFINES.TILE_SIZE) / texture->getSize().y);
    this->sprite->setPosition(this->coords.x, this->coords.y);
}

// getters
Coords Tile::getCoords()
{
    return this->coords;
}

Position Tile::getPosition()
{
    return this->position;
}

sf::Sprite& Tile::getSprite()
{
    return *this->sprite;
}

// setters
void Tile::setCoords(Coords coords)
{
    this->coords = coords;
}

void Tile::setPosition(Position position)
{
    this->position = position;
}

// methods

void Tile::draw(sf::RenderWindow& window)
{
    if(this->sprite)
    {
        auto a = this->sprite->getGlobalBounds();
        window.draw(*this->sprite);
    }
}
