#include <SFML/Graphics.hpp>
#include <iostream>

#include "headers/tile.hpp"
#include "headers/config.hpp"

Tile::Tile(int c, int r, texture_ptr dotImage, texture_ptr superDotImage, Type type, Contents content)
{
    this->c = c;
    this->r = r;
    this->type = type;
    this->pacman = content == Contents::pacman ? true : false;
    this->ghost = content == Contents::ghost ? true : false;
    this->dot = content == Contents::dot ? true : false;
    this->superDot = content == Contents::superdot ? true : false;

    this->dotImage = dotImage;
    this->dotSprite = std::make_unique<sf::Sprite>(*this->dotImage);
    this->superDotImage = superDotImage;
    this->superDotSprite = std::make_unique<sf::Sprite>(*this->superDotImage);

    this->x = DEFINES.MAP_MARGIN_SIDE + this->c * DEFINES.TILE_SIZE;
    this->y = DEFINES.MAP_MARGIN_TOP + this->r * DEFINES.TILE_SIZE;

    this->dotSprite->setScale(DEFINES.TILE_SIZE / this->dotImage->getSize().x, DEFINES.TILE_SIZE / this->dotImage->getSize().y);
    this->superDotSprite->setScale(DEFINES.TILE_SIZE / this->superDotImage->getSize().x, DEFINES.TILE_SIZE / this->superDotImage->getSize().y);
    this->dotSprite->setPosition(this->x, this->y);
    this->superDotSprite->setPosition(this->x, this->y);
}

sf::Vector2i Tile::getCoords()
{
    return {this->c, this->r};
}

sf::Vector2f Tile::getPosition()
{
    return {this->x, this->y};
}

bool Tile::containsPacman()
{
    return this->pacman;
}

bool Tile::containsGhost()
{
    return this->ghost;
}

bool Tile::containsDot()
{
    return this->dot;
}

bool Tile::containsSuperDot()
{
    return this->superDot;
}

void Tile::setContent(Contents content)
{
    switch(content)
    {
    case Contents::pacman:
        this->pacman = true;
        this->dot = false;          //pacman eats the dot
        this->superDot = false;         //pacman eats the super dot
        break;
    case Contents::ghost:
        this->ghost = true;
        break;
    case Contents::none:
        this->pacman = false;
        this->ghost = false;
        this->dot = false;
        this->superDot = false;
        break;
    }
}

bool Tile::isWall()
{
    return this->type == Type::wall || this->type == Type::ghosthouseDoor;
}

bool Tile::isTunel()
{
    return this->type == Type::tunel;
}

void Tile::draw(sf::RenderWindow& window)
{
    if(this->dot)
        window.draw(*this->dotSprite);
    else if(this->superDot)
        window.draw(*this->superDotSprite);
}
