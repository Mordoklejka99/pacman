#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "headers/config.hpp"
#include "headers/ghost.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/pacman.hpp"

int distance(sf::Vector2f lv, sf::Vector2f rv)
{
    return abs(lv.x - rv.x) + abs(lv.y - rv.y);
}

Ghost::Ghost(float x, float y, float speed, std::shared_ptr<sf::Texture>& ghostImage, std::shared_ptr<sf::Sprite>& ghostSprite, std::shared_ptr<Map>& map)
{
    this->coords = {x, y};
    this->position = {-1, -1};
    this->speed = speed;
    this->radius = DEFINES.GHOST_SIZE / 2;
    this->ghostImage = ghostImage;
    this->ghostSprite = ghostSprite;
    this->moveDirection = Direction::none;
    this->map = map;

    this->ghostSprite->setPosition(this->coords.x, this->coords.y);
}

Direction Ghost::getDirection()
{
    return this->moveDirection;
}

sf::Vector2f Ghost::getCoords()
{
    return {this->coords.x, this->coords.y};
}

sf::Vector2i Ghost::getPosition()
{
    return {this->position.c, this->position.r};
}

sf::Vector2i Ghost::findTilePosition(sf::Vector2f pos)
{
    // coords of center of ghost
    float px = pos.x + DEFINES.GHOST_SIZE / 2;
    float py = pos.y + DEFINES.GHOST_SIZE / 2;

    for(int c = 0; c < this->map->getWidth(); c++)
    {
        for(int r = 0; r < this->map->getHeight(); r++)
        {
            //coords of top left corner of tile
            float tx = (*this->map)(c, r).getCoords().x;
            float ty = (*this->map)(c, r).getCoords().y;

            if(px > tx && px < tx + DEFINES.TILE_SIZE && py > ty && py < ty + DEFINES.TILE_SIZE)
            {
                if(!(*this->map)(c, r).isWall())
                    return {c, r};
                else
                    return {-1, -1};
            }
        }
    }
}

Direction Ghost::findPath(sf::Vector2f destination)
{
    Direction direction = this->getDirection();
    Direction result;
    int dist = (map->getHeight() + map->getWidth() + 4) * DEFINES.TILE_SIZE;
    
    if(this->getPosition() == sf::Vector2i(-1, -1))
    {
        for(int dir = 1; dir < int(Direction::nOfDirections); dir++)
        {
            sf::Vector2i position = this->findTilePosition(this->getCoords() + Directions[int(dir)]);
            if(position != sf::Vector2i(-1, -1))
            {
                Tile& tile = (*this->map)(position);
                int currDist = distance(tile.getCoords(), destination);
                if(currDist < dist)
                {
                    this->position.c = position.x;
                    this->position.r = position.y;
                    dist = currDist;
                    result = Direction(dir);
                }
            }
        }
    }
    else
    {
        for(int dir = 1; dir < int(Direction::nOfDirections); dir++)
        {
            if(Directions[dir] == -Directions[int(this->moveDirection)])
                continue;
            Tile& tile = (*this->map)(this->getPosition() + sf::Vector2i(Directions[int(dir)]));
            int currDist = distance(tile.getCoords(), destination);
            if(!tile.isWall() && currDist < dist)
            {
                dist = currDist;
                result = Direction(dir);
            }
        }
    }
    return result;
}

void Ghost::move(Pacman& pacman)
{
    if(this->position.c == -1 || this->position.r == -1)
        this->moveDirection = this->findPath(pacman.getCoords());

    Tile& tile = (*this->map)(this->getPosition());
    Direction dir = this->moveDirection;

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
        else
        {
            this->moveDirection = Direction(int(this->moveDirection) * -1);
        }
    }
    // if about to hit, the wall, TURN
    else if((*this->map)(this->getPosition() + sf::Vector2i(Directions[int(dir)])).isWall()
        && this->coords.x + Directions[int(dir)].x * this->speed < tile.getCoords().x + 2
        && this->coords.x + Directions[int(dir)].x * this->speed > tile.getCoords().x - 2
        && this->coords.y + Directions[int(dir)].y * this->speed < tile.getCoords().y + 2
        && this->coords.y + Directions[int(dir)].y * this->speed > tile.getCoords().y - 2)
    {
        this->coords = {tile.getCoords().x + 1, tile.getCoords().y + 1};
        this->moveDirection = this->findPath(this->getDestination(pacman));
    }
    // otherwise, go ahead
    else
    {
        if(this->coords.x + Directions[int(dir)].x * this->speed < tile.getCoords().x + 2
            && this->coords.x + Directions[int(dir)].x * this->speed > tile.getCoords().x - 2
            && this->coords.y + Directions[int(dir)].y * this->speed < tile.getCoords().y + 2
            && this->coords.y + Directions[int(dir)].y * this->speed > tile.getCoords().y - 2)
        {
            this->coords = {tile.getCoords().x + 1, tile.getCoords().y + 1};
            this->moveDirection = this->findPath(this->getDestination(pacman));
        }
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
    }

    Tile& tile_ = (*this->map)(this->getPosition());
    tile.unsetContent(Contents::ghost);
    tile_.setContent(Contents::ghost);
    this->ghostSprite->setPosition(this->getCoords());

    std::cerr << "--" << std::endl;
    std::cerr << "| " << this->position.c << " " << this->position.r << std::endl;
    std::cerr << "| " << pacman.getPosition().x << " " << pacman.getPosition().y << std::endl;
    std::cerr << "--" << std::endl;
}

void Ghost::draw(sf::RenderWindow& window)
{
    window.draw(*this->ghostSprite);
}
