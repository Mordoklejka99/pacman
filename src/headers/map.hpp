#ifndef MAP_HPP
#define MAP_HPP

#include <memory>
#include <SFML/Graphics.hpp>
#include "config.hpp"

class Tile;

class Map
{
private:
    uint width;
    uint height;
    uint dotCount;
    Tile*** tiles;
    std::unique_ptr<sf::Texture> bkgTexture;
    std::unique_ptr<sf::Sprite> bkgSprite;

public:
    // ctor
    Map(MapData& mapData);

    // dtor
    ~Map();

    // getters
    uint getWidth();
    uint getHeight();
    uint getDotCount();

    // operators
    Tile& operator()(uint row, uint col);
    Tile& operator()(Position position);
    void operator--(int);

    // methods
    bool offTheMap(Position position) const noexcept;
    void draw(sf::RenderWindow& window);
};

#endif
