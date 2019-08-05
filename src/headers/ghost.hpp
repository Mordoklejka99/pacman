#ifndef GHOST_HPP
#define GHOST_HPP

#include <memory>
#include <SFML/Graphics.hpp>

#include "config.hpp"

enum class Direction;
class Map;
class Pacman;

class Ghost
{
    struct
    {
        float x;
        float y;
    } coords;
    struct
    {
        int c;
        int r;
    } position;
    float speed;
    float radius;
    Direction moveDirection;
    std::shared_ptr<Map> map;

    std::shared_ptr<sf::Texture> ghostImage;
    std::shared_ptr<sf::Sprite> ghostSprite;

public:
    Ghost(float x, float y, float speed, std::shared_ptr<sf::Texture>& ghostImage, std::shared_ptr<sf::Sprite>& ghostSprite, std::shared_ptr<Map>& map);
    void move(Pacman& pacman);
    void draw(sf::RenderWindow& window);
    Direction getDirection();
    sf::Vector2f getCoords();
    sf::Vector2i getPosition();

private:
    sf::Vector2i findTilePosition(sf::Vector2f pos);
    virtual sf::Vector2f getDestination(Pacman& pacman) const = 0;
    Direction findPath(sf::Vector2f destination);
};

#endif
