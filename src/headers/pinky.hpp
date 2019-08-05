#ifndef PINKY_HPP
#define PINKY_HPP

#include "ghost.hpp"
#include "config.hpp"

enum class Direction;
class Map;
class Pacman;

class Pinky : public Ghost
{
public:
    Pinky(MapData::Pinky pinky, float speed, std::shared_ptr<Map>& map);

private:
    virtual sf::Vector2f getDestination(Pacman& pacman) const override;
};

#endif
