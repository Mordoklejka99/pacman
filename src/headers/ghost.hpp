#ifndef GHOST_HPP
#define GHOST_HPP

#include "config.hpp"

class Map;
class Pacman;

class Ghost
{
protected:
    Coords coords;
    Position position;
    Direction moveDirection;
    float speed;
    bool hasMoved;
    sf::Sprite* sprite;
    Map& map;
    Pacman& pacman;
    bool** bMap;
    int** iMap;

public:
    // ctor
    Ghost(MapData& mapData, Map& map, Pacman& pacman, sf::Texture* texture);

    // dtor
    ~Ghost();

    // getters
    Coords getCoords() const;
    Position getPosition() const;
    Direction getMoveDirection() const;

    // methods
    void move();
    void draw(sf::RenderWindow& window) const;

protected:
    bool findTilePosition(Coords coords);
    bool isAtBorder() const;
    void zeroiMap();
    void dijkstra(Position position);
    virtual Position getDestination() const = 0;
    Direction chooseDirection();
};

#endif
