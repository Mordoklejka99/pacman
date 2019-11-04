// standard c++ lib
#include <iostream>
#include <vector>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "headers/pacman.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/ghost.hpp"


// ctor

Pacman::Pacman(MapData& mapData, Map& map) : map(map)
{
    this->coords = mapData.pacman.coords;

    this->sprite = new sf::Sprite(*CONFIG.pacmanTexture);
    this->sprite->setScale(float(DEFINES.TILE_SIZE) / CONFIG.pacmanTexture->getSize().x, float(DEFINES.TILE_SIZE) / CONFIG.pacmanTexture->getSize().y);
    this->sprite->setPosition(this->coords.x, this->coords.y);

    this->faceDirection = this->moveDirection = Direction::none;
    this->moved = false;
    this->dead = false;
}


// dtor
Pacman::~Pacman()
{
    delete this->sprite;
}


// getters

Coords Pacman::getCoords() const
{
    return this->coords;
}

Position Pacman::getPosition() const
{
    return this->position;
}

Direction Pacman::getMoveDirection() const
{
    return this->moveDirection;
}

Direction Pacman::getFaceDirection() const
{
    return this->faceDirection;
}

bool Pacman::hasMoved() const
{
    return this->moved;
}

bool Pacman::isDead() const
{
    return this->dead;
}

bool Pacman::isOnDrugs() const
{
    return this->onDrugs;
}

int Pacman::getScore() const
{
    return this->score;
}


// methods

void Pacman::turn(Direction dir)
{
    // if has moved, just plan turning
    if(this->moved)
        this->plannedTurn = dir;
    // otherwise try to move
    else
    {
        if(this->findTilePosition(this->coords + Directions[int(dir)]))
        {
            this->faceDirection = this->moveDirection = this->plannedTurn = dir;
            this->moved = true;
            this->map.timer.restart();
        }
    }
}

void Pacman::move()
{
    // if first move has not been done, stand by
    if(!this->moved)
        return;

    if(this->onDrugs && this->timer.getElapsedTime().asMilliseconds() >= 7000)
    {
        this->onDrugs = false;
        this->map.blinky->changeMode(GhostMode::chase);
        this->map.pinky->changeMode(GhostMode::chase);
        this->map.inky->changeMode(GhostMode::chase);
        this->map.clyde->changeMode(GhostMode::chase);
    }

    // to shorten conditions at least a bit
    Tile& currTile = this->map(this->position);
    Direction dir = this->plannedTurn;

    // if it's possible to turn, turn
    try
    {
        if(!this->map(this->position + Directions[int(dir)]).isWall()
            && Directions[int(dir)] == -Directions[int(this->moveDirection)])
        {
            this->faceDirection = this->moveDirection = dir;
        }
        else if(!this->map(this->position + Directions[int(dir)]).isWall()
            && this->coords.x < currTile.getCoords().x + 2
            && this->coords.x > currTile.getCoords().x - 2
            && this->coords.y < currTile.getCoords().y + 2
            && this->coords.y > currTile.getCoords().y - 2
            && dir != this->moveDirection)
        {
            this->coords = currTile.getCoords();
            this->faceDirection = this->moveDirection = dir;
        }
    }
    catch(InvalidTilePositionException)
    {
        if(currTile.isTunel()
            && this->coords.x < currTile.getCoords().x + 2
            && this->coords.x > currTile.getCoords().x - 2
            && this->coords.y < currTile.getCoords().y + 2
            && this->coords.y > currTile.getCoords().y - 2
            && dir != this->moveDirection)
        {
            this->coords = currTile.getCoords();
            this->faceDirection = this->moveDirection = dir;
        }
    }

    dir = this->moveDirection;
    float speed;

    if(this->onDrugs)
        speed = DEFINES.PACMAN_SPEED.ON_DRUGS * this->sprite->getScale().x;
    else
        speed = DEFINES.PACMAN_SPEED.NORMAL * this->sprite->getScale().x;

    // move
    // if there's a tunel, go through it
    if(this->isAtBorder() && currTile.isTunel())
    {
        // still don't go through the wall, though
        try
        {
            if(this->map(this->position + sf::Vector2f(Directions[int(dir)])).isWall()
                && this->coords.x + Directions[int(dir)].x * speed < currTile.getCoords().x + 2
                && this->coords.x + Directions[int(dir)].x * speed > currTile.getCoords().x - 2
                && this->coords.y + Directions[int(dir)].y * speed < currTile.getCoords().y + 2
                && this->coords.y + Directions[int(dir)].y * speed > currTile.getCoords().y - 2)
            {
                this->coords = currTile.getCoords();
                dir = this->moveDirection = Direction::none;
            }
        }
        catch(InvalidTilePositionException)
        {
            //
        }
        
        // change coords
        this->coords += Directions[int(dir)] * speed;

        // if center of pacman has passed tile (and map) edge, teleport it to the other side
        Coords center(this->coords + Coords(DEFINES.TILE_SIZE / 2));
        if(center.x < this->map(0, 0).getCoords().x
            || center.x > this->map(this->map.getWidth() - 1, 0).getCoords().x + DEFINES.TILE_SIZE
            || center.y < this->map(0, 0).getCoords().y
            || center.y > this->map(0, this->map.getHeight() - 1).getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position = (this->position + Position(this->map.getWidth(), this->map.getHeight()) + Directions[int(dir)])
                                % sf::Vector2i(this->map.getWidth(), this->map.getHeight());
            this->coords = this->map(this->position).getCoords() + currTile.getCoords() - this->coords;
        }

        // if moving towards center of the map and passed tile edge, update position
        else if(center.x < currTile.getCoords().x
            || center.x > currTile.getCoords().x + DEFINES.TILE_SIZE
            || center.y < currTile.getCoords().y
            || center.y > currTile.getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position += Directions[int(dir)];
        }
    }
    // if about to hit the wall, STOP
    else if(this->map(this->position + sf::Vector2f(Directions[int(dir)])).isWall()
        && this->coords.x + Directions[int(dir)].x * speed < currTile.getCoords().x + 2
        && this->coords.x + Directions[int(dir)].x * speed > currTile.getCoords().x - 2
        && this->coords.y + Directions[int(dir)].y * speed < currTile.getCoords().y + 2
        && this->coords.y + Directions[int(dir)].y * speed > currTile.getCoords().y - 2)
    {
        this->coords = currTile.getCoords();
        this->moveDirection = Direction::none;
    }
    // otherwise, go ahead
    else
    {
        this->coords += Directions[int(dir)] * speed;
        if(this->coords.x + DEFINES.TILE_SIZE / 2 < currTile.getCoords().x
            || this->coords.x + DEFINES.TILE_SIZE / 2 > currTile.getCoords().x + DEFINES.TILE_SIZE
            || this->coords.y + DEFINES.TILE_SIZE / 2 < currTile.getCoords().y
            || this->coords.y + DEFINES.TILE_SIZE / 2 > currTile.getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position += Directions[int(dir)];
        }
    }

    // updating currTile
    Tile& newCurrTile = this->map(this->position);

    if(newCurrTile.containsDot())
    {
        this->score += 10;
        this->map--;
        this->map.timer.restart();
        this->map.dotCounter++;
    }
    else if(newCurrTile.containsSuperDot())
    {
        this->score += 50;
        this->map--;
        this->map.timer.restart();
        this->map.dotCounter++;
        this->timer.restart();
        this->onDrugs = true;
        this->ghostsEatenMultiplier = 0;
        this->map.blinky->changeMode(GhostMode::frightened);
        this->map.pinky->changeMode(GhostMode::frightened);
        this->map.inky->changeMode(GhostMode::frightened);
        this->map.clyde->changeMode(GhostMode::frightened);
    }

    currTile.setPacman(false);
    newCurrTile.setPacman(true);
    
    if(newCurrTile.containsGhost())
    {
        std::vector<Ghost*> ghosts = this->map.ghostsInTile(this->position);
        for(Ghost* ghost : ghosts)
        {
            GhostMode mode = ghost->getMode();
            if(mode == GhostMode::dead)
                continue;
            if(mode == GhostMode::frightened)
            {
                this->ghostsEatenMultiplier = (this->ghostsEatenMultiplier == 0 ? 1 : 2 * this->ghostsEatenMultiplier);
                this->score += (this->ghostsEatenMultiplier) * 200;
                ghost->changeMode(GhostMode::dead);
                continue;
            }
            this->dead = true;
        }
    }

    this->sprite->setPosition(this->coords.x, this->coords.y);
}

bool Pacman::findTilePosition(Coords coords)
{
    // coords of center of pacman
    Coords center(coords + Coords(DEFINES.TILE_SIZE / 2));

    for(int c = 0; c < this->map.getWidth(); c++)
    {
        for(int r = 0; r < this->map.getHeight(); r++)
        {
            //coords of top left corner of tile
            Coords corner(this->map(c, r).getCoords().x, this->map(c, r).getCoords().y);

            if(center.x > corner.x && center.x < corner.x + DEFINES.TILE_SIZE
                && center.y > corner.y && center.y < corner.y + DEFINES.TILE_SIZE)
            {
                if(this->map(c, r).isWall())
                {
                    return false;
                }
                else
                {
                    this->position = Position(c, r);
                    return true;
                }
            }
        }
    }
    return false;
}

bool Pacman::isAtBorder()
{
    Tile& tile = this->map(this->position);
    return tile.getPosition().c == 0 || tile.getPosition().c == this->map.getWidth() - 1
            || tile.getPosition().r == 0 || tile.getPosition().r == this->map.getHeight() - 1;
}

void Pacman::draw(sf::RenderWindow& window) const
{
    window.draw(*this->sprite);
}
