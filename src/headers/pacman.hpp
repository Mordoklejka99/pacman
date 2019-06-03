#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <memory>

class Map;

enum class Direction
{
    left,
    up,
    right,
    down,
    none
};

class Pacman
{
    int score;
    float x;
    float y;
    int c;
    int r;
    float radius;
    float speed;
    Direction moveDirection;
    Direction plannedTurn;
    std::unique_ptr<sf::CircleShape> player;
    std::shared_ptr<Map> map;

public:
    Pacman(float x, float y, float radius, float speed, std::shared_ptr<Map>& map);
    ~Pacman() {};
    void setDirection(Direction dir);
    sf::Vector2f getPosition();
    int getScore();
    void turn(Direction dir);
    void move();
    void draw(sf::RenderWindow& window);

private:
    sf::Vector2i findTilePosition(float x, float y);
};

#endif
