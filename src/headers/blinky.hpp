#ifndef BLINKY_HPP
#define BLINKY_HPP

#include "config.hpp"
#include "ghost.hpp"

class Blinky : public Ghost
{
public:
    // ctor
    Blinky(MapData& mapData, Map& map, Pacman& pacman);

private:
    virtual Position getDestination() const;
};

#endif
