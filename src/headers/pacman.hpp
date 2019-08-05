#ifndef PACMAN_HPP
#define PACMAN_HPP

#include <memory>

enum class Direction;
class Map;

class Pacman
{
    int score;
    bool dead;
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
    Direction getDirection();
    void setCoords(float x, float y);
    sf::Vector2f getCoords();
    void setPosition(int c, int r);
    sf::Vector2i getPosition();
    int getScore();
    bool isDead();
    void revive();
    void turn(Direction dir);
    void move();
    void draw(sf::RenderWindow& window);

private:
    sf::Vector2i findTilePosition(sf::Vector2f position);
};

#endif
