// standard c++ lib
#include <iostream>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/tile.hpp"
#include "headers/config.hpp"
#include "headers/ghost.hpp"


// ctors

Tile::Tile(Position position, TileContents content)
{
    this->position = position;
    this->coords.x = DEFINES.SIDE_MARGIN + float(this->position.c) * DEFINES.TILE_SIZE;
    this->coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + float(this->position.r) * DEFINES.TILE_SIZE;
    this->content = content;

    this->pacman = false;
    this->ghost = false;

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

TileContents Tile::getContent()
{
    return this->content;
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

void Tile::setPacman(bool state)
{
    this->pacman = state;
    if(this->pacman && (this->containsDot() || this->containsSuperDot()))
    {
        delete this->sprite;
        this->sprite = nullptr;
        this->content = TileContents::none;
    }
}

void Tile::setGhost(bool state)
{
    this->ghost = state;
}

// methods

bool Tile::isWall()
{
    return this->content == TileContents::wall || this->content == TileContents::ghosthouseDoor;
}

bool Tile::isGhosthouseDoor()
{
    return this->content == TileContents::ghosthouseDoor;
}

bool Tile::isGhosthouse()
{
    return this->content == TileContents::ghosthouse;
}

bool Tile::isTunel()
{
    return this->content == TileContents::tunel;
}

bool Tile::containsDot()
{
    return this->content == TileContents::dot;
}

bool Tile::containsSuperDot()
{
    return this->content == TileContents::superDot;
}

bool Tile::containsGhost()
{
    return this->ghost;
}

void Tile::draw(sf::RenderWindow& window)
{
    if(this->sprite)
    {
        window.draw(*this->sprite);
    }
}
