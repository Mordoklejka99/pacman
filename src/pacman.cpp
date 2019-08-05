#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "headers/pacman.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/config.hpp"

Pacman::Pacman(float x, float y, float radius, float speed, std::shared_ptr<Map>& map)
{
    this->score = 0;
    this->dead = false;
    this->coords = {x, y};
    this->position = {-1, -1};
    this->radius = radius;
    this->speed = speed;
    this->moveDirection = Direction::none;
    this->plannedTurn = Direction::none;

    this->player = std::make_unique<sf::CircleShape>(this->radius);
    this->player->setPosition(this->coords.x, this->coords.y);
    this->player->setFillColor(sf::Color::Yellow);

    this->map = map;
}

sf::Vector2i Pacman::findTilePosition(sf::Vector2f pos)
{
    // coords of center of pacman
    float px = pos.x + this->radius;
    float py = pos.y + this->radius;

    for(int c = 0; c < this->map->getWidth(); c++)
    {
        for(int r = 0; r < this->map->getHeight(); r++)
        {
            //coords of top left corner of tile
            float tx = (*this->map)(c, r).getCoords().x;
            float ty = (*this->map)(c, r).getCoords().y;

            if(px > tx && px < tx + DEFINES.TILE_SIZE && py > ty && py < ty + DEFINES.TILE_SIZE)
                if(!(*this->map)(c, r).isWall())
                    return {c, r};
                else
                    return {-1, -1};
        }
    }
}

void Pacman::setDirection(Direction dir)
{
    this->moveDirection = dir;
}

Direction Pacman::getDirection()
{
    return this->moveDirection;
}

sf::Vector2f Pacman::getCoords()
{
    return {this->coords.x, this->coords.y};
}

void Pacman::setCoords(float x, float y)
{
    this->coords = {x, y};
}

sf::Vector2i Pacman::getPosition()
{
    return {this->position.c, this->position.r};
}

void Pacman::setPosition(int c, int r)
{
    this->position = {c, r};
}

int Pacman::getScore()
{
    return this->score;
}

bool Pacman::isDead()
{
    return this->dead;
}

void Pacman::revive()
{
    this->dead = false;
}

void Pacman::turn(Direction dir)
{
    if(this->position.c == -1 || this->position.r == -1)
    {
        // sf::Vector2i position = this->findTilePosition(this->getPosition().x + Directions[int(dir)].x, this->getPosition().y + Directions[int(dir)].y);
        sf::Vector2i position = this->findTilePosition(this->getCoords() + Directions[int(dir)]);
        if(position != sf::Vector2i(-1, -1))
        {
            this->position.c = position.x;
            this->position.r = position.y;
            this->plannedTurn = dir;
        }
    }
    else
        this->plannedTurn = dir;
}

void Pacman::move()
{
    if(this->position.c == -1 || this->position.r == -1)      // don't move if first turn wasn't made
        return;

    Tile& tile = (*this->map)(this->getPosition());        // to shorten conditions
    Direction dir = this->plannedTurn;

    if(this->moveDirection == Direction::none
        && !(*this->map)(this->getPosition() + sf::Vector2i(Directions[int(dir)])).isWall())
    {
        this->moveDirection = dir;
    }  

    // if it is possible to turn to plannedTurn, turn there
    if(this->position.c > 0
        && this->position.c < this->map->getWidth() - 1
        && this->position.r > 0
        && this->position.r < this->map->getHeight() - 1
        && !(*this->map)(this->getPosition() + sf::Vector2i(Directions[int(dir)])).isWall()
        && this->coords.x < tile.getCoords().x + 2
        && this->coords.x > tile.getCoords().x - 2
        && this->coords.y < tile.getCoords().y + 2
        && this->coords.y > tile.getCoords().y - 2)
    {
        if(dir != this->moveDirection)
            this->coords = {tile.getCoords().x + 1, tile.getCoords().y + 1};
        this->moveDirection = dir;
    }

    dir = this->moveDirection;

    // move
    // if it's edge of a map, STOP
    if(this->position.c - 1 < 0
        || this->position.c + 1 >= this->map->getWidth()
        || this->position.r - 1 < 0
        || this->position.r + 1 >= this->map->getHeight())
    {
        // BUT if it's tunnel, go through it
        if(tile.isTunel())
        {
            this->coords.x += Directions[int(dir)].x * this->speed;
            this->coords.y += Directions[int(dir)].y * this->speed;
            // if center of pacman passed tile edge, move it to the other side
            if(this->coords.x + this->radius < (*this->map)(0, 0).getCoords().x
                || this->coords.x + this->radius > (*this->map)(this->map->getWidth() - 1, 0).getCoords().x + DEFINES.TILE_SIZE
                || this->coords.y + this->radius < (*this->map)(0, 0).getCoords().y
                || this->coords.y + this->radius > (*this->map)(0, this->map->getHeight() - 1).getCoords().y + DEFINES.TILE_SIZE)
            {
                this->position.c = ((this->position.c + int(Directions[int(dir)].x)) % this->map->getWidth() + this->map->getWidth()) % this->map->getWidth();
                this->position.r = ((this->position.r + int(Directions[int(dir)].y)) % this->map->getHeight() + this->map->getHeight()) % this->map->getHeight();
                this->coords.x = (*this->map)(this->getPosition()).getCoords().x + tile.getCoords().x - this->coords.x + 2;
                this->coords.y = (*this->map)(this->getPosition()).getCoords().y + tile.getCoords().y - this->coords.y + 2;
            }
            else if(this->coords.x + this->radius < tile.getCoords().x
                || this->coords.x + this->radius > tile.getCoords().x + DEFINES.TILE_SIZE
                || this->coords.y + this->radius < tile.getCoords().y
                || this->coords.y + this->radius > tile.getCoords().y + DEFINES.TILE_SIZE)
            {
                this->position.c += Directions[int(dir)].x;
                this->position.r += Directions[int(dir)].y;
            }
        }
    }
    // if about to hit the wall, STOP
    else if((*this->map)(this->getPosition() + sf::Vector2i(Directions[int(dir)])).isWall()
        && this->coords.x + Directions[int(dir)].x * this->speed < tile.getCoords().x + 2
        && this->coords.x + Directions[int(dir)].x * this->speed > tile.getCoords().x - 2
        && this->coords.y + Directions[int(dir)].y * this->speed < tile.getCoords().y + 2
        && this->coords.y + Directions[int(dir)].y * this->speed > tile.getCoords().y - 2)
    {
        this->coords = {tile.getCoords().x + 1, tile.getCoords().y + 1};
        this->moveDirection = Direction::none;
    }
    // otherwise, go ahead
    else
    {
        this->coords.x += Directions[int(dir)].x * this->speed;
        this->coords.y += Directions[int(dir)].y * this->speed;
        if(this->coords.x + this->radius < tile.getCoords().x
            || this->coords.x + this->radius > tile.getCoords().x + DEFINES.TILE_SIZE
            || this->coords.y + this->radius < tile.getCoords().y
            || this->coords.y + this->radius > tile.getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position.c += Directions[int(dir)].x;
            this->position.r += Directions[int(dir)].y;
        }
    }

    Tile& tile_ = (*this->map)(this->getPosition());

    if(tile_.containsGhost())
        this->dead = true;
    else if(tile_.containsDot())
    {
        this->score += 10;
        (*this->map)--;
    }
    else if(tile_.containsSuperDot())
    {
        this->score += 50;
        (*this->map)--;
    }

    tile.unsetContent(Contents::pacman);
    tile_.setContent(Contents::pacman);
    this->player->setPosition(this->getCoords());
}

void Pacman::draw(sf::RenderWindow& window)
{
    window.draw(*(this->player));
}
