#ifndef PACMAN_HPP
#define PACMAN_HPP

#include "config.hpp"

class Map;

class Pacman
{
private:
    Coords coords;
    Position position;
    Direction moveDirection;
    Direction plannedTurn;
    float speed;
    uint score;
    bool dead;
    bool hasMoved;
    sf::Sprite* sprite;
    Map& map;

public:
    // ctor
    Pacman(MapData& mapData, Map& map);

    // getters
    Coords getCoords() const;
    Position getPosition() const;
    bool isDead() const;
    uint getScore() const;

    // methods
    void turn(Direction dir);
    void move();
    void draw(sf::RenderWindow& window) const;

private:
    bool findTilePosition(Coords coords);
    bool isAtBorder();
};

#endif
