#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <memory>

using uint = unsigned int;

namespace sf
{
    class Texture;
};

class Tile;

struct Coords
{
    float x;
    float y;

    friend std::ostream& operator<<(std::ostream& out, Coords rval)
    {
        out << rval.x << " " << rval.y;
        return out;
    }
};

struct Position
{
    uint c;
    uint r;

    Position()
    {
        this->c = 0;
        this->r = 0;
    }
    Position(uint c, uint r)
    {
        this->c = c;
        this->r = r;
    }
    Position& operator=(Position& rval)
    {
        this->c = rval.c;
        this->r = rval.r;
        return *this;
    }
    bool operator==(Position rval)
    {
        return this->c == rval.c && this->r == rval.r;
    }
};

struct Resolution
{
    float width;
    float height;
};

enum class TileContents
{
    none,
    tunel,
    dot,
    superDot,
    wall,
    ghosthouseDoor
};

struct Defines
{
    float SIDE_MARGIN;
    float TOP_MARGIN;
    float HUD_MARGIN;
    float TILE_SIZE;
};

struct Config
{
    Resolution resolution;
    bool fullscreen;
    sf::Texture* dotTexture;
    sf::Texture* superDotTexture;
    sf::Texture* wallTexture;
};

struct MapData
{
    uint width;
    uint height;
    struct
    {
        uint x;
        uint y;
    } pacman;
    struct
    {
        uint x;
        uint y;
    } inky;
    struct
    {
        uint x;
        uint y;
    } blinky;
    struct
    {
        uint x;
        uint y;
    } pinky;
    struct
    {
        uint x;
        uint y;
    } clyde;
    Tile*** tiles;
    uint dotCount;
    bool** map;
};

bool loadConfigFile();
bool loadTextures();
bool loadMap(MapData& mapData);

extern Defines DEFINES;
extern Config CONFIG;

#endif
