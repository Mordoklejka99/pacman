#ifndef GHOST_HPP
#define GHOST_HPP

#include <utility>
#include <queue>
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
    bool changedTile;
    sf::Sprite* sprite;
    Map& map;
    Pacman& pacman;
    int** bMap;
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
    void wallDijkstra(std::queue<std::pair<Position, int>>& q, Position position);
    void offMapDijkstra(std::queue<std::pair<Position, int>>& q, Position position);
    void dijkstra(Position position);
    virtual Position getDestination() const = 0;
    Direction chooseDirection();
};


class Blinky : public Ghost
{
public:
    // ctor
    Blinky(MapData& mapData, Map& map, Pacman& pacman);

private:
    virtual Position getDestination() const override;
};


class Pinky : public Ghost
{
public:
    // ctor
    Pinky(MapData& mapData, Map& map, Pacman& pacman);

private:
    virtual Position getDestination() const override;
};


class Inky : public Ghost
{
private:
    Blinky& blinky;
public:
    // ctor
    Inky(MapData& mapData, Map& map, Pacman& pacman, Blinky& blinky);

private:
    virtual Position getDestination() const override;
};


class Clyde : public Ghost
{
public:
    // ctor
    Clyde(MapData& mapData, Map& map, Pacman& pacman);

private:
    virtual Position getDestination() const override;
};

#endif
