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
    this->x = x;
    this->y = y;
    this->c = -1;
    this->r = -1;
    this->radius = radius;
    this->speed = speed;
    this->moveDirection = Direction::none;
    this->plannedTurn = Direction::none;

    this->player = std::make_unique<sf::CircleShape>(this->radius);
    this->player->setPosition(this->x, this->y);
    this->player->setFillColor(sf::Color::Yellow);

    this->map = map;
}

sf::Vector2i Pacman::findTilePosition(float x, float y)
{
    // coords of center of pacman
    float px = this->x + this->radius;
    float py = this->y + this->radius;

    for(int c = 0; c < this->map->getWidth(); c++)
    {
        for(int r = 0; r < this->map->getHeight(); r++)
        {
            //coords of top left corner of tile
            float tx = (*this->map)(c, r).getPosition().x;
            float ty = (*this->map)(c, r).getPosition().y;

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

sf::Vector2f Pacman::getPosition()
{
    return {this->x, this->y};
}

int Pacman::getScore()
{
    return this->score;
}

void Pacman::turn(Direction dir)
{
    if(this->c == -1 || this->r == -1)
    {
        sf::Vector2i position;
        switch(dir)
        {
        case Direction::left:
            position = this->findTilePosition(this->x - 1, this->y);
            if(position == sf::Vector2i(-1, -1))
                return;
            else
            {
                this->c = position.x;
                this->r = position.y;

                this->plannedTurn = dir;
            }
            break;
        case Direction::right:
            position = this->findTilePosition(this->x + 1, this->y);
            if(position == sf::Vector2i(-1, -1))
                return;
            else
            {
                this->c = position.x;
                this->r = position.y;

                this->plannedTurn = dir;
            }
            break;
        case Direction::up:
            position - this->findTilePosition(this->x, this->y - 1);
            if(position == sf::Vector2i(-1, -1))
                return;
            else
            {
                this->c = position.x;
                this->r = position.y;

                this->plannedTurn = dir;
            }
        case Direction::down:
            position - this->findTilePosition(this->x, this->y + 1);
            if(position == sf::Vector2i(-1, -1))
                return;
            else
            {
                this->c = position.x;
                this->r = position.y;

                this->plannedTurn = dir;
            }
        }
    }
    else
    {
        switch(dir)
        {
        case Direction::left:
            this->plannedTurn = dir;
            return;
        case Direction::right:
            this->plannedTurn = dir;
            return;
        case Direction::up:
            this->plannedTurn = dir;
            return;
        case Direction::down:
            this->plannedTurn = dir;
            return;
        }
    }
}

void Pacman::move()
{
    if(this->c == -1 || this->r == -1)      // don't move if first turn wasn't made
        return;

    Tile& tile = (*this->map)(this->c, this->r);        // to shorten conditions
    
    // if it is possible to turn to plannedTurn, turn there
    switch(this->plannedTurn)
    {
    case Direction::none:
        return;
    case Direction::up:
        if(this->r != 0 && !(*this->map)(this->c, this->r - 1).isWall() && this->x < tile.getPosition().x + 2 && this->x > tile.getPosition().x - 2)
        {
            this->x = tile.getPosition().x + 1;
            this->moveDirection = Direction::up;
        }
        break;
    case Direction::down:
        if(this->r != this->map->getHeight() -1 && !(*this->map)(this->c, this->r + 1).isWall() && this->x < tile.getPosition().x + 2 && this->x > tile.getPosition().x - 2)
        {
            this->x = tile.getPosition().x + 1;
            this->moveDirection = Direction::down;
        }
        break;
    case Direction::left:
        if(this->c != 0 && !(*this->map)(this->c - 1, this->r).isWall() && this->y < tile.getPosition().y + 2 && this->y > tile.getPosition().y - 2)
        {
            this->y = tile.getPosition().y + 1;
            this->moveDirection = Direction::left;
        }
        break;
    case Direction::right:
        if(this->c != this->map->getWidth() - 1 && !(*this->map)(this->c +1, this->r).isWall() && this->y < tile.getPosition().y + 2 && this->y > tile.getPosition().y - 2)
        {
            this->y = tile.getPosition().y + 1;
            this->moveDirection = Direction::right;
        }
        break;
    }

    // move
    switch(this->moveDirection)
    {
    case Direction::up:
        if(this->r == 0)    // if hit top of the map, STOP
        {
            if(tile.isTunel())      //but if you're in tunel, go ahead
            {
                this->y -= this->speed;
                if(this->y + this->radius < tile.getPosition().y)
                {
                    this->r = this->map->getHeight() - 1;
                    this->y = (*this->map)(this->c, this->r).getPosition().y + DEFINES.TILE_SIZE;
                }
                break;
            }
            this->moveDirection = Direction::none;
            break;
        }
        if((*this->map)(this->c, this->r - 1).isWall() && this->y - this->speed < tile.getPosition().y)     //if you hit wall, STOP
        {
            this->y = tile.getPosition().y + 1;
            this->moveDirection = Direction::none;
        }
        else    // otherwise you're free to go
        {
            this->y -= this->speed;
            if(this->y + this->radius < tile.getPosition().y)       // if pacman center passed tiles border, change tile coords
                this->r--;
        }
        break;
    // comments analogical later on
    case Direction::down:
        if(this->r == this->map->getHeight() - 1)
        {
            if(tile.isTunel())      //but if you're in tunel, go ahead
            {
                this->y += this->speed;
                if(this->y + this->radius > tile.getPosition().y + DEFINES.TILE_SIZE)
                {
                    this->r = 0;
                    this->y = (*this->map)(this->c, this->r).getPosition().y;
                }
                break;
            }
            this->moveDirection = Direction::none;
            break;
        }
        if((*this->map)(this->c, this->r + 1).isWall() && this->y + 2 * this->radius + this->speed > (*this->map)(this->c, this->r + 1).getPosition().y)
        {
            this->y = tile.getPosition().y + 1;
            this->moveDirection = Direction::none;
        }
        else
        {
            this->y += this->speed;
            if(this->y + this->radius > (*this->map)(this->c, this->r + 1).getPosition().y)
                this->r++;
        }
        break;
    case Direction::left:
        if(this->c == 0)
        {
            if(tile.isTunel())      //but if you're in tunel, go ahead
            {
                this->x -= this->speed;
                if(this->x + this->radius < tile.getPosition().x)
                {
                    this->c = this->map->getWidth() - 1;
                    this->x = (*this->map)(this->c, this->r).getPosition().x + DEFINES.TILE_SIZE;
                }
                break;
            }
            this->moveDirection = Direction::none;
            break;
        }
        if((*this->map)(this->c - 1, this->r).isWall() && this->x - this->speed < tile.getPosition().x)
        {
            this->x = tile.getPosition().x + 1;
            this->moveDirection = Direction::none;
        }
        else
        {
            this->x -= this->speed;
            if(this->x + this->radius < tile.getPosition().x)
                this->c--;
        }
        break;
    case Direction::right:
        if(this->c == this->map->getWidth() - 1)
        {
            if(tile.isTunel())      //but if you're in tunel, go ahead
            {
                this->x += this->speed;
                if(this->x + this->radius > tile.getPosition().x + DEFINES.TILE_SIZE)
                {
                    this->c = 0;
                    this->x = (*this->map)(this->c, this->r).getPosition().x;
                }
                break;
            }
            this->moveDirection = Direction::none;
            break;
        }
        if((*this->map)(this->c + 1, this->r).isWall() && this->x + 2 * this->radius + this->speed > (*this->map)(this->c + 1, this->r).getPosition().x)
        {
            this->x = tile.getPosition().x + 1;
            this->moveDirection = Direction::none;
        }
        else
        {
            this->x += this->speed;
            if(this->x + this->radius > (*this->map)(this->c + 1, this->r).getPosition().x)
                this->c++;
        }
        break;
    }
    if(tile.containsDot())
        this->score += 10;
    else if(tile.containsSuperDot())
        this->score += 50;
    tile.setContent(Contents::pacman);
    this->player->setPosition(this->x, this->y);
}

void Pacman::draw(sf::RenderWindow& window)
{
    window.draw(*(this->player));
}
