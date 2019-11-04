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
    Position respawnPosition;
    Direction moveDirection;
    Direction plannedTurn;
    GhostMode mode;
    bool hasMoved;
    bool changedTile;
    bool isOut;
    bool dead;
    bool wasDead;
    sf::Sprite* normalSprite;
    sf::Sprite* frightenedSprite;
    sf::Sprite* deadSprite;
    sf::Sprite* currentSprite;
    Map& map;
    int** bMap;
    int** iMap;
    static sf::Clock timer;

public:
    // ctor
    Ghost(MapData& mapData, Map& map, Textures textures);
    void reconstruct(MapData& mapData, Map& map);

    // dtor
    ~Ghost();

    // getters
    Coords getCoords() const;
    Position getPosition() const;
    Direction getMoveDirection() const;
    GhostMode getMode() const;

    // methods
    void move();
    void changeMode(GhostMode mode);
    void draw(sf::RenderWindow& window) const;

protected:
    bool findTilePosition(Direction dir);
    bool isAtBorder() const;
    void zeroiMap();
    void wallDijkstra(std::queue<std::pair<Position, int>>& q, Position position);
    void offMapDijkstra(std::queue<std::pair<Position, int>>& q, Position position);
    void dijkstra(Position position);
    virtual Position getDestination() const = 0;
    virtual bool mayLeave() const = 0;
    Direction chooseDirection();
};


class Blinky : public Ghost
{
public:
    // ctor
    Blinky(MapData& mapData, Map& map);
    void reconstruct(MapData& mapData, Map& map);

private:
    virtual Position getDestination() const override;
    virtual bool mayLeave() const override;
};


class Pinky : public Ghost
{
public:
    // ctor
    Pinky(MapData& mapData, Map& map);
    void reconstruct(MapData& mapData, Map& map);

private:
    virtual Position getDestination() const override;
    virtual bool mayLeave() const override;
};


class Inky : public Ghost
{
public:
    // ctor
    Inky(MapData& mapData, Map& map);
    void reconstruct(MapData& mapData, Map& map);

private:
    virtual Position getDestination() const override;
    virtual bool mayLeave() const override;
};


class Clyde : public Ghost
{
public:
    // ctor
    Clyde(MapData& mapData, Map& map);
    void reconstruct(MapData& mapData, Map& map);

private:
    virtual Position getDestination() const override;
    virtual bool mayLeave() const override;
};

#endif
