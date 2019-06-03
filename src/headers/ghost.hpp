#ifndef GHOST_HPP
#define GHOST_HPP

#include <memory>
#include <SFML/Graphics.hpp>

class MapData;
class Map;

class Ghost
{
    float x;
    float y;
    int c;
    int r;
    float speed;
    std::shared_ptr<Map> map;

    std::shared_ptr<sf::Texture> ghostImage;
    std::shared_ptr<sf::Sprite> ghostSprite;
public:
    Ghost(float x, float y, float speed, std::shared_ptr<sf::Texture>& ghostImage, std::shared_ptr<sf::Sprite>& ghostSprite, std::shared_ptr<Map>& map);
    void draw(sf::RenderWindow& window);
};

class Inky : public Ghost
{
public:
    Inky(MapData::Inky inky, float speed, std::shared_ptr<Map>& map) : Ghost(inky.x, inky.y, speed, inky.ghostImage, inky.ghostSprite, map) {};
};

class Blinky : public Ghost
{
public:
    Blinky(MapData::Blinky blinky, float speed, std::shared_ptr<Map>& map) : Ghost(blinky.x, blinky.y, speed, blinky.ghostImage, blinky.ghostSprite, map) {};
};

class Pinky : public Ghost
{
public:
    Pinky(MapData::Pinky pinky, float speed, std::shared_ptr<Map>& map) : Ghost(pinky.x, pinky.y, speed, pinky.ghostImage, pinky.ghostSprite, map) {};
};

class Clyde : public Ghost
{
public:
    Clyde(MapData::Clyde clyde, float speed, std::shared_ptr<Map>& map) : Ghost(clyde.x, clyde.y, speed, clyde.ghostImage, clyde.ghostSprite, map) {};
};

#endif
