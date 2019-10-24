#ifndef TILE_HPP
#define TILE_HPP

#include <SFML/Graphics.hpp>
#include "config.hpp"

class Tile
{
private:
    Coords coords;
    Position position;
    TileContents content;
    sf::Sprite* sprite;

    bool pacman;
    bool ghost;

public:
    // ctors
    Tile(Position position, TileContents content);
    Tile(Position position, TileContents content, sf::Texture* texture);

    // getters
    Coords getCoords();
    Position getPosition();
    TileContents getContent();
    sf::Sprite& getSprite();

    // setters
    void setCoords(Coords coords);
    void setPosition(Position position);
    void setPacman(bool state);
    void setGhost(bool state);

    // methods
    bool isWall();
    bool isGhosthouseDoor();
    bool isTunel();
    bool containsDot();
    bool containsSuperDot();
    bool containsGhost();
    void draw(sf::RenderWindow& window);
};

#endif
