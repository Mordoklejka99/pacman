#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "ghost.hpp"
#include "config.hpp"

enum class Direction;
class Map;
class Pacman;

class Blinky : public Ghost
{
public:
    Blinky(MapData::Blinky blinky, float speed, std::shared_ptr<Map>& map);

private:
    virtual sf::Vector2f getDestination(Pacman& pacman) const override;
};

#endif
