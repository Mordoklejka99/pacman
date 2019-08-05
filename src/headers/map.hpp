#ifndef MAP_HPP
#define MAP_HPP

#include <memory>

class Tile;
struct MapData;

class Map
{
    int width;
    int height;
    int dotCount;
    Tile*** tiles;
    std::shared_ptr<sf::Texture> backgroundImage;
    std::shared_ptr<sf::Sprite> background;

public:
    Map(std::shared_ptr<MapData>& map);
    ~Map();
    int getWidth();
    int getHeight();
    int dotsLeft();
    void draw(sf::RenderWindow& window);

    Tile& operator()(int c, int r);
    Tile& operator()(sf::Vector2i pos);
    int operator--(int);
};

#endif
