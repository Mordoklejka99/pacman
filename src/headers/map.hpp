#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Tile;
class Pacman;
class Ghost;
class Blinky;
class Pinky;
class Inky;
class Clyde;

class DotCounter
{
private:
    int dotCount = 0;

public:
    int getCount()
    {
        return this->dotCount;
    }
    void restart()
    {
        this->dotCount = 0;
    }
    void operator++(int)
    {
        this->dotCount++;
    }
};

class Map
{
public:
    sf::Clock timer;
    DotCounter dotCounter;
    Pacman* pacman;
    Blinky* blinky;
    Pinky* pinky;
    Inky* inky;
    Clyde* clyde;

private:
    int width;
    int height;
    int dotCount;
    int nOfDots;
    Tile*** tiles;
    std::unique_ptr<sf::Texture> bkgTexture;
    std::unique_ptr<sf::Sprite> bkgSprite;
    bool pinkyOut;
    bool inkyOut;
    bool clydeOut;

public:
    // ctor
    Map(MapData& mapData);

    // dtor
    ~Map();

    // getters
    int getWidth();
    int getHeight();
    int getDotCount();
    int getTotalDotCount();

    // operators
    Tile& operator()(int row, int col);
    Tile& operator()(Position position);
    void operator--(int);

    // methods
    bool offTheMap(Position position) const;
    std::vector<Ghost*> ghostsInTile(Position position);
    void move();
    void draw(sf::RenderWindow& window) const;
};

#endif
