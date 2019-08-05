#ifndef INKY_HPP
#define INKY_HPP

#include "ghost.hpp"
#include "config.hpp"

enum class Direction;
class Map;
class Pacman;

class Inky : public Ghost
{
public:
    Inky(MapData::Inky inky, float speed, std::shared_ptr<Map>& map);

private:
    virtual sf::Vector2f getDestination(Pacman& pacman) const override;
};

#endif
