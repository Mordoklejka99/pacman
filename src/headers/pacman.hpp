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
    Direction faceDirection;
    Direction plannedTurn;
    int score;
    int ghostsEatenMultiplier;
    bool dead;
    bool moved;
    bool onDrugs;
    sf::Sprite* sprite;
    Map& map;
    sf::Clock timer;

public:
    // ctor
    Pacman(MapData& mapData, Map& map);
    void reconstruct(MapData& mapData, Map& map);

    // dtor
    ~Pacman();

    // getters
    Coords getCoords() const;
    Position getPosition() const;
    Direction getMoveDirection() const;
    Direction getFaceDirection() const;
    bool hasMoved() const;
    bool isDead() const;
    bool isOnDrugs() const;
    int getScore() const;

    // methods
    void turn(Direction dir);
    void move();
    void draw(sf::RenderWindow& window) const;

private:
    bool findTilePosition(Coords coords);
    bool isAtBorder();
};

#endif
