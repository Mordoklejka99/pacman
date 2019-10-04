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

    // getters
    uint getWidth();
    uint getHeight();
    uint getDotCount();

    // operators
    Tile& operator()(uint row, uint col);
    void operator--();

    // methods
    void draw(sf::RenderWindow& window);
};

#endif
