#ifndef TILE_HPP
#define TILE_HPP

#include <memory>
#include <SFML/Graphics.hpp>

using texture_ptr = std::shared_ptr<sf::Texture>;
using sprite_ptr = std::unique_ptr<sf::Sprite>;

enum class Type
{
    wall,
    ghosthouse,
    ghosthouseDoor,
    tunel,
    empty
};

enum class Contents
{
    pacman,
    ghost,
    dot,
    superdot,
    none
};

class Tile
{
    int c;
    int r;
    float x;
    float y;
    Type type;
    bool pacman;
    bool ghost;
    bool dot;
    bool superDot;

    texture_ptr dotImage;
    sprite_ptr dotSprite;
    texture_ptr superDotImage;
    sprite_ptr superDotSprite;

public:
    Tile(int c, int r, texture_ptr dotImage, texture_ptr superDotImage, Type type, Contents content = Contents::none);
    sf::Vector2i getCoords();
    sf::Vector2f getPosition();
    bool containsPacman();
    bool containsGhost();
    bool containsDot();
    bool containsSuperDot();
    void setContent(Contents content);
    bool isWall();
    bool isTunel();

    void draw(sf::RenderWindow& window);
};

#endif
