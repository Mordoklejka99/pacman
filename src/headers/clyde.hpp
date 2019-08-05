#ifndef CLYDE_HPP
#define CLYDE_HPP

#include "ghost.hpp"
#include "config.hpp"

enum class Direction;
class Map;
class Pacman;

class Clyde : public Ghost
{
public:
    Clyde(MapData::Clyde clyde, float speed, std::shared_ptr<Map>& map);

private:
    virtual sf::Vector2f getDestination(Pacman& pacman) const override;
};

#endif
