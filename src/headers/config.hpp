#ifndef CONFIG_HPP
#define CONFIG_HPP

// #include <SFML/Graphics.hpp>
#include <exception>

using uint = unsigned int;

namespace sf
{
    class Texture;
    class Sprite;
    class RenderWindow;

    template <typename T>
    class Vector2;

    typedef Vector2<float> Vector2f;
    typedef Vector2<int> Vector2i;
};

class Tile;

template <typename T>
sf::Vector2<T> operator* (float val, sf::Vector2<T> v)
{
    return sf::Vector2<T>(val * v.x, val * v.y);
}

struct Resolution
{
    float width;
    float height;
};

enum class Direction
{
    none = 0,
    up,
    down,
    left,
    right,
    nOfDirections
};

enum class GhostMode
{
    idle = 0,
    scatter,
    chase,
    frightened,
    dead
};

class InvalidTilePositionException : public std::exception
{
public:
    const char* what() const throw()
    {
        return "Invalid tile position";
    }
};

struct Coords
{
    float x;
    float y;

    // ctors
    Coords();
    Coords(float val);
    Coords(float x, float y);
    Coords(sf::Vector2f& v);

    // operators
    Coords& operator=(Coords rval);
    bool operator==(Coords& rval);
    Coords operator+(Coords rval);
    Coords operator+(sf::Vector2f rval);
    Coords operator-(Coords rval);
    Coords& operator+=(Coords rval);
    Coords& operator+=(sf::Vector2f rval);
    friend std::ostream& operator<<(std::ostream& out, Coords rval);
};

struct Position
{
    int c;
    int r;

    // ctors
    Position();
    Position(int c, int r);
    Position& operator=(Position rval);

    // operators
    bool operator==(Position& rval);
    Position operator+(Position rval);
    Position operator+(sf::Vector2f rval);
    Position operator-(Position rval);
    Position operator-(sf::Vector2f rval);
    Position operator%(sf::Vector2i rval);
    Position& operator+=(sf::Vector2f rval);
    friend std::ostream& operator<<(std::ostream& out, Position rval);
};

enum class TileContents
{
    none,
    tunel,
    dot,
    superDot,
    wall,
    ghosthouseDoor,
    ghosthouse
};

struct Defines
{
    float SIDE_MARGIN;
    float TOP_MARGIN;
    float HUD_MARGIN;
    float TILE_SIZE;
    const float PACMAN_SPEED = 2.;
    const float GHOST_SPEED = 1.7;
};

struct Textures
{
    sf::Texture* normalTexture;
    sf::Texture* frightenedTexture;
    sf::Texture* deadTexture;

    Textures()
    {
        this->normalTexture = new sf::Texture;
        this->frightenedTexture = new sf::Texture;
        this->deadTexture = new sf::Texture;
    }
};

struct Config
{
    Resolution resolution;
    bool fullscreen;
    sf::Texture* dotTexture;
    sf::Texture* superDotTexture;
    sf::Texture* wallTexture;
    sf::Texture* pacmanTexture;
    Textures blinkyTextures;
    Textures inkyTextures;
    Textures pinkyTextures;
    Textures clydeTextures;
};

struct MapData
{
    int width;
    int height;
    struct
    {
        Coords coords;
    } pacman;
    struct
    {
        Coords coords;
        Position respawn;
    } inky;
    struct
    {
        Coords coords;
        Position respawn;
    } blinky;
    struct
    {
        Coords coords;
        Position respawn;
    } pinky;
    struct
    {
        Coords coords;
        Position respawn;
    } clyde;
    Tile*** tiles;
    int dotCount;
    int** map;
};

int distance(Position lhv, Position rhv);

bool loadConfigFile();
bool loadTextures();
bool loadMap(MapData& mapData);

extern Defines DEFINES;
extern Config CONFIG;

extern sf::Vector2f Directions[int(Direction::nOfDirections)];

#endif
