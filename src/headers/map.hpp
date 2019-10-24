#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Tile;

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
};

class Map
{
public:
    sf::Clock timer;
    DotCounter dotCounter;

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
    bool pinkyIsOut() const;
    bool inkyIsOut() const;
    bool clydeIsOut() const;
    void draw(sf::RenderWindow& window);
};

#endif
