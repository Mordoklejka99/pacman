#ifndef TILE_HPP
#define TILE_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Tile
{
private:
    Coords coords;
    Position position;
    TileContents content;
    sf::Sprite* sprite;

public:
    // ctors
    Tile(Position position, TileContents content);
    Tile(Position position, TileContents content, sf::Texture* texture);

    // getters
    Coords getCoords();
    Position getPosition();
    sf::Sprite& getSprite();

    // setters
    void setCoords(Coords coords);
    void setPosition(Position position);

    // methods
    void draw(sf::RenderWindow& window);
};

#endif
