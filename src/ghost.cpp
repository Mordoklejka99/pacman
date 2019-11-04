// standard c++ lib
#include <iostream>
#include <iomanip>
#include <queue>
#include <cmath>
#include <cstdlib>

// additional libs
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

// project headers
#include "headers/ghost.hpp"
#include "headers/config.hpp"
#include "headers/map.hpp"
#include "headers/tile.hpp"
#include "headers/pacman.hpp"


sf::Clock Ghost::timer = sf::Clock();

// Ghost
// ctor
Ghost::Ghost(MapData& mapData, Map& map, Textures textures) : map(map)
{
    this->bMap = mapData.map;
    this->iMap = new int*[this->map.getHeight()];
    for(int c = 0; c < this->map.getHeight(); c++)
        this->iMap[c] = new int[this->map.getWidth()];

    this->normalSprite = new sf::Sprite(*textures.normalTexture);
    this->normalSprite->setScale(float(DEFINES.TILE_SIZE) / textures.normalTexture->getSize().x, float(DEFINES.TILE_SIZE) / textures.normalTexture->getSize().y);
    this->frightenedSprite = new sf::Sprite(*textures.frightenedTexture);
    this->frightenedSprite->setScale(this->normalSprite->getScale());
    this->deadSprite = new sf::Sprite(*textures.deadTexture);
    this->deadSprite->setScale(this->normalSprite->getScale());

    this->currentSprite = this->normalSprite;

    this->moveDirection = Direction::none;
    this->mode = GhostMode::idle;
    this->hasMoved = false;
    this->changedTile = false;
    this->isOut = false;
    this->dead = false;
    this->wasDead = false;
}


// dtor
Ghost::~Ghost()
{
    for(int c = 0; c < this->map.getWidth(); c++)
    {
        delete[] this->iMap[c];
        delete[] this->bMap[c];
    }
    delete[] this->iMap;
    delete[] this->bMap;

    delete this->normalSprite;
    delete this->frightenedSprite;
}


// getters
Coords Ghost::getCoords() const
{
    return this->coords;
}

Position Ghost::getPosition() const
{
    return this->position;
}

Direction Ghost::getMoveDirection() const
{
    return this->moveDirection;
}

GhostMode Ghost::getMode() const
{
    return this->mode;
}


// methods
void Ghost::move()
{
    // don't move if pacman hasn't moved A.K.A. game hasn't started yet
    if(!this->map.pacman->hasMoved())
        return;
        
    // don't move if in idle mode (waiting to join the party)
    if(this->mode == GhostMode::idle)
    {
        if(this->mayLeave())
        {
            this->isOut = true;
            this->mode = GhostMode::chase;
        }
        else
            return;
    }

    // if hasn't moved yet, find out where you are
    if(!this->hasMoved)
    {
        for(int dir = int(Direction::up); dir != int(Direction::nOfDirections); dir++)
        {
            bool found = this->findTilePosition(Direction(dir));
            if(found)
                break;
        }
    }

    // to shorten conditions at least a bit
    Tile& currTile = this->map(this->position);
    Direction dir = this->plannedTurn = this->chooseDirection();

    // if it's possible to turn, turn
    try
    {
        if(!this->map(this->position + Directions[int(dir)]).isWall()
            && Directions[int(dir)] == -Directions[int(this->moveDirection)])
        {
            this->moveDirection = dir;
            this->changedTile = false;
        }
        else if((!this->map(this->position + Directions[int(dir)]).isWall()
                || (this->map(this->position + Directions[int(dir)]).isGhosthouseDoor()
                    && (this->map(this->position).isGhosthouse()
                        || this->dead)))
            && this->coords.x < currTile.getCoords().x + 2
            && this->coords.x > currTile.getCoords().x - 2
            && this->coords.y < currTile.getCoords().y + 2
            && this->coords.y > currTile.getCoords().y - 2
            && dir != this->moveDirection)
        {
            this->coords = currTile.getCoords();
            this->moveDirection = dir;
            this->changedTile = false;
        }
        else if(this->map(this->position + Directions[int(this->moveDirection)]).isWall()
            && !this->map(this->position + Directions[int(this->moveDirection)]).isGhosthouseDoor()
            && (!this->map(this->position + Directions[int(dir)]).isWall()
                || this->map(this->position + Directions[int(dir)]).isGhosthouseDoor()))
        {
            this->moveDirection = dir;
            this->changedTile = false;
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
            this->moveDirection = dir;
        }
    }

    dir = this->moveDirection;
    float speed;

    if(this->mode == GhostMode::frightened)
        speed = DEFINES.GHOST_SPEED.FRIGHTENED * this->currentSprite->getScale().x;
    else if(currTile.isTunel() && this->mode != GhostMode::dead)
        speed = DEFINES.GHOST_SPEED.IN_TUNEL * this->currentSprite->getScale().x;
    else
        speed = DEFINES.GHOST_SPEED.NORMAL * this->currentSprite->getScale().x;

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

        // if center of ghost has passed tile (and map) edge, teleport it to the other side
        Coords center(this->coords + Coords(DEFINES.TILE_SIZE / 2));
        if(center.x < this->map(0, 0).getCoords().x
            || center.x > this->map(this->map.getWidth() - 1, 0).getCoords().x + DEFINES.TILE_SIZE
            || center.y < this->map(0, 0).getCoords().y
            || center.y > this->map(0, this->map.getHeight() - 1).getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position = (this->position + Position(this->map.getWidth(), this->map.getHeight()) + Directions[int(dir)])
                                % sf::Vector2i(this->map.getWidth(), this->map.getHeight());
            this->coords = this->map(this->position).getCoords() + currTile.getCoords() - this->coords;
            this->changedTile = true;
        }

        // if moving towards center of the map and passed tile edge, update position
        else if(center.x < currTile.getCoords().x
            || center.x > currTile.getCoords().x + DEFINES.TILE_SIZE
            || center.y < currTile.getCoords().y
            || center.y > currTile.getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position += Directions[int(dir)];
            this->changedTile = true;
        }
    }
    //if about to EXIT ghosthouse, go for it
    else if(this->map(this->position + Directions[int(dir)]).isGhosthouseDoor() && this->isOut)
    {
        this->coords += Directions[int(dir)] * speed;
        if(this->coords.x + DEFINES.TILE_SIZE / 2 < currTile.getCoords().x
            || this->coords.x + DEFINES.TILE_SIZE / 2 > currTile.getCoords().x + DEFINES.TILE_SIZE
            || this->coords.y + DEFINES.TILE_SIZE / 2 < currTile.getCoords().y
            || this->coords.y + DEFINES.TILE_SIZE / 2 > currTile.getCoords().y + DEFINES.TILE_SIZE)
        {
            this->position += Directions[int(dir)];
            this->changedTile = true;
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
            this->changedTile = true;
        }
    }

    // updating currTile
    Tile& newCurrTile = this->map(this->position);

    if(&currTile != &newCurrTile)
    {
        currTile.setGhost(false);
        newCurrTile.setGhost(true);
    }
    if(this->dead && this->position == this->respawnPosition)
    {
        this->mode = GhostMode::chase;
        this->dead = false;
        this->currentSprite = this->normalSprite;
    }

    this->currentSprite->setPosition(this->coords.x, this->coords.y);
}

bool Ghost::findTilePosition(Direction dir)
{
    // coords of center of ghost
    Coords center(this->coords + Directions[int(dir)] + Coords(DEFINES.TILE_SIZE / 2));

    for(int c = 0; c < this->map.getWidth(); c++)
    {
        for(int r = 0; r < this->map.getHeight(); r++)
        {
            //coords of top left corner of tile
            Coords corner(this->map(c, r).getCoords().x, this->map(c, r).getCoords().y);

            if(center.x > corner.x && center.x < corner.x + DEFINES.TILE_SIZE
                && center.y > corner.y && center.y < corner.y + DEFINES.TILE_SIZE)
            {
                if(!this->map(c, r).isWall())
                {
                    this->position = Position(c, r);
                    this->moveDirection = dir;
                    this->hasMoved = true;
                    this->changedTile = true;
                    return true;
                }
            }
        }
    }
    return false;
}

void Ghost::changeMode(GhostMode mode)
{
    switch(mode)
    {
    case GhostMode::idle:
        this->mode = mode;
        this->currentSprite = this->normalSprite;
        break;
    case GhostMode::scatter:
        if(this->isOut && this->mode != GhostMode::dead && this->mode != GhostMode::scatter)
        this->mode = mode;
        this->currentSprite = this->normalSprite;
        break;
    case GhostMode::chase:
        if(this->isOut && this->mode != GhostMode::dead && this->mode != GhostMode::chase)
        {
            this->mode = mode;
            this->currentSprite = this->normalSprite;
        }
        break;
    case GhostMode::frightened:
        if(this->mode != GhostMode::idle && this->mode != GhostMode::dead && this->mode != GhostMode::frightened
            && !this->map(this->position).isGhosthouse() && !this->map(this->position).isGhosthouseDoor())
        {
            this->mode = mode;
            this->changedTile = true;
            this->currentSprite = this->frightenedSprite;
            for(int i = 1; i < int(Direction::nOfDirections); i++)
            {
                auto dir = Directions[i];
                if(dir == -Directions[int(this->moveDirection)])
                {
                    this->moveDirection = Direction(i);
                    break;
                }
            }
        }
        break;
    case GhostMode::dead:
        this->mode = mode;
        this->currentSprite = this->deadSprite;
        this->dead = true;
        break;
    }
}

bool Ghost::isAtBorder() const
{
    Tile& tile = this->map(this->position);
    return tile.getPosition().c == 0 || tile.getPosition().c == this->map.getWidth() - 1
            || tile.getPosition().r == 0 || tile.getPosition().r == this->map.getHeight() - 1;
}

void Ghost::zeroiMap()
{
    for(int c = 0; c < this->map.getWidth(); c++)
        for(int r = 0; r < this->map.getHeight(); r++)
            this->iMap[c][r] = INT32_MAX;
}

void Ghost::wallDijkstra(std::queue<std::pair<Position, int>>& tileQueue, Position destination)
{
    std::queue<std::pair<Position, int>> wallQueue;
    wallQueue.push(std::make_pair(destination, 0));

    int closestTile = INT32_MAX;
    bool foundTile = false;

    while(wallQueue.size())
    {
        auto pos = wallQueue.front();
        wallQueue.pop();

        // if found better path to tile, proceed
        if(pos.second < this->iMap[pos.first.c][pos.first.r])
        {
            this->iMap[pos.first.c][pos.first.r] = pos.second;

            // for every adjacent tile
            for(auto dir : Directions)
            {
                // don't process tile that's off the map
                if(this->map.offTheMap(pos.first + dir))
                    continue;

                // add closest to destination non-wall tile
                if(this->bMap[pos.first.c + int(dir.x)][pos.first.r + int(dir.y)] == 1 && pos.second + 1 <= closestTile)
                {
                    foundTile = true;
                    closestTile = pos.second + 1;
                    tileQueue.push(std::make_pair(pos.first + dir, closestTile));
                    continue;
                }
                // add wall tile adjacent in dir
                if(!foundTile && this->bMap[pos.first.c + int(dir.x)][pos.first.r + int(dir.y)] == 0)
                {
                    wallQueue.push(std::make_pair(pos.first + dir, pos.second + 1));
                }
            }
        }
    }
}

void Ghost::offMapDijkstra(std::queue<std::pair<Position, int>>& tileQueue, Position destination)
{
    Position pos = destination;

    if(pos.c < 0)
        pos.c = 0;
    if(pos.r < 0)
        pos.r = 0;
    if(pos.c >= this->map.getWidth())
        pos.c = this->map.getWidth() - 1;
    if(pos.r >= this->map.getHeight())
        pos.r = this->map.getHeight() - 1;

    this->wallDijkstra(tileQueue, pos);
}

void Ghost::dijkstra(Position destination)
{
    this->zeroiMap();

    std::queue<std::pair<Position, int>> tileQueue;

    if(this->map.offTheMap(destination))
        this->offMapDijkstra(tileQueue, destination);
    else if(this->bMap[destination.c][destination.r] == 0)
        this->wallDijkstra(tileQueue, destination);
    else
        tileQueue.push(std::make_pair(destination, 0));
    
    while(tileQueue.size())
    {
        auto pos = tileQueue.front();
        tileQueue.pop();

        // if found better path to tile, proceed
        if(pos.second < this->iMap[pos.first.c][pos.first.r])
        {
            this->iMap[pos.first.c][pos.first.r] = pos.second;

            // for every adjacent tile
            for(auto dir : Directions)
            {
                // add tile the other side of a tunel
                if(this->map(pos.first).isTunel()
                    && (pos.first.c + dir.x < 0
                            || pos.first.c + dir.x > this->map.getWidth() - 1
                            || pos.first.r + dir.y < 0
                            || pos.first.r + dir.y > this->map.getHeight() - 1))
                {
                    Position p = (pos.first + Position(this->map.getWidth(), this->map.getHeight()) + dir)
                                    % sf::Vector2i(this->map.getWidth(), this->map.getHeight());
                    tileQueue.push(std::make_pair(p, pos.second + 1));
                    continue;
                }
                // do not add tile that's outside of the map
                if(this->map.offTheMap(pos.first + dir))
                    continue;
                // add tile adjecent in dir
                if(this->bMap[pos.first.c + int(dir.x)][pos.first.r + int(dir.y)] == 1)
                {
                    tileQueue.push(std::make_pair(pos.first + dir, pos.second + 1));
                }
            }
        }
    }
}

Direction Ghost::chooseDirection()
{
    // don't turn if hadn't move to another tile after last turn
    if(!this->changedTile && this->moveDirection != Direction::none)
        return this->moveDirection;

    Tile& currTile = this->map(this->position);
    
    if(this->mode == GhostMode::frightened)
    {
        if(!(this->coords.x < currTile.getCoords().x + 2
            && this->coords.x > currTile.getCoords().x - 2
            && this->coords.y < currTile.getCoords().y + 2
            && this->coords.y > currTile.getCoords().y - 2))
        {
            return this->moveDirection;
        }

        while(true)
        {
            int dir = rand() % (int(Direction::nOfDirections) - 1) + 1;
            try
            {
                if(Directions[dir] == -Directions[int(this->moveDirection)]
                    || this->map(this->position + Directions[dir]).isWall())
                {
                    continue;
                }
            }
            catch(InvalidTilePositionException)
            {
                if(Directions[dir] != -Directions[int(this->moveDirection)]
                    && currTile.isTunel())
                {
                    return Direction(dir);
                }
            }
            return Direction(dir);
        }
    }

    // else
    Direction shortestDir = this->moveDirection;
    int shortestLen = INT32_MAX;
    Position backPos = (this->position + Position(this->map.getWidth(), this->map.getHeight()) - Directions[int(shortestDir)])
                        % sf::Vector2i(this->map.getWidth(), this->map.getHeight());
    if(this->bMap[backPos.c][backPos.r] == 0)
        this->dijkstra(this->getDestination());
    else
    {
        this->bMap[backPos.c][backPos.r] = 2;
        this->dijkstra(this->getDestination());
        this->bMap[backPos.c][backPos.r] = 1;
    }

    for(int i = 1; i < int(Direction::nOfDirections); i++)
    {
        try
        {
            auto dir = Directions[i];

            if(dir == -Directions[int(this->moveDirection)])
                continue;

            Position pos = (this->position + Position(this->map.getWidth(), this->map.getHeight()) + dir)
                                % sf::Vector2i(this->map.getWidth(), this->map.getHeight());

            if((!this->map(pos).isWall()
                || (this->map(pos).isGhosthouseDoor()
                    && (this->map(this->position).isGhosthouse()
                        || this->mode == GhostMode::dead)))
                && this->iMap[pos.c][pos.r] < shortestLen
                && (this->moveDirection == Direction::none
                    || (this->coords.x < currTile.getCoords().x + 2
                        && this->coords.x > currTile.getCoords().x - 2
                        && this->coords.y < currTile.getCoords().y + 2
                        && this->coords.y > currTile.getCoords().y - 2)))
            {
                shortestLen = this->iMap[pos.c][pos.r];
                shortestDir = Direction(i);
                this->changedTile = false;
            }
        }
        catch(InvalidTilePositionException)
        {
            //
        }
    }
    return shortestDir;
}

void Ghost::draw(sf::RenderWindow& window) const
{
    window.draw(*this->currentSprite);
}


// Blinky
// ctor
Blinky::Blinky(MapData& mapData, Map& map)
        : Ghost(mapData, map, CONFIG.blinkyTextures)
{
    this->coords = mapData.blinky.coords;
    this->respawnPosition = mapData.blinky.respawn;
    this->normalSprite->setPosition(this->coords.x, this->coords.y);
    this->frightenedSprite->setPosition(this->coords.x, this->coords.y);
    this->mode = GhostMode::chase;
    this->isOut = true;
}

// methods
Position Blinky::getDestination() const
{
    if(this->dead)
        return this->respawnPosition;
    else
        return this->map.pacman->getPosition();
}

bool Blinky::mayLeave() const
{
    if(this->wasDead)
        return this->timer.getElapsedTime().asMilliseconds() > 1000;
    else
        return true;
}


// Pinky
// ctor
Pinky::Pinky(MapData& mapData, Map& map)
        : Ghost(mapData, map, CONFIG.pinkyTextures)
{
    this->coords = mapData.pinky.coords;
    this->respawnPosition = mapData.pinky.respawn;
    this->normalSprite->setPosition(this->coords.x, this->coords.y);
    this->frightenedSprite->setPosition(this->coords.x, this->coords.y);
}

// methods
Position Pinky::getDestination() const
{
    if(this->dead)
        return this->respawnPosition;
    else
        return this->map.pacman->getPosition() + 4 * Directions[int(this->map.pacman->getFaceDirection())];
}

bool Pinky::mayLeave() const
{
    if(this->wasDead)
    {
        return this->timer.getElapsedTime().asMilliseconds() > 1000;
    }
    if(this->timer.getElapsedTime().asMilliseconds() >= 1000)
    {
        this->timer.restart();
        std::cerr << "Pinky" << std::endl;
        return true;
    }
    return false;
}


// Inky
// ctor
Inky::Inky(MapData& mapData, Map& map)
        : Ghost(mapData, map, CONFIG.inkyTextures)
{
    this->coords = mapData.inky.coords;
    this->respawnPosition = mapData.inky.respawn;
    this->normalSprite->setPosition(this->coords.x, this->coords.y);
    this->frightenedSprite->setPosition(this->coords.x, this->coords.y);
}

// methods
Position Inky::getDestination() const
{
    if(this->dead)
        return this->respawnPosition;
    else
    {
        Position offsetPos = this->map.pacman->getPosition() + 2 * Directions[int(this->map.pacman->getFaceDirection())];
        Position diff = offsetPos - this->map.blinky->getPosition();
        return offsetPos + diff;
    }
}

bool Inky::mayLeave() const
{
    if(this->wasDead)
    {
        return this->timer.getElapsedTime().asMilliseconds() > 1000;
    }
    if(this->timer.getElapsedTime().asMilliseconds() >= 4000
        && (this->map.timer.getElapsedTime().asMilliseconds() >= 4000 || this->map.dotCounter.getCount() >= 30))
    {
        this->map.timer.restart();
        this->map.dotCounter.restart();
        std::cerr << "Inky" << std::endl;
        return true;
    }
    return false;
}


// Clyde
// ctor
Clyde::Clyde(MapData& mapData, Map& map)
        : Ghost(mapData, map, CONFIG.clydeTextures)
{
    this->coords = mapData.clyde.coords;
    this->respawnPosition = mapData.clyde.respawn;
    this->normalSprite->setPosition(this->coords.x, this->coords.y);
    this->frightenedSprite->setPosition(this->coords.x, this->coords.y);
}

// methods
Position Clyde::getDestination() const
{
    if(this->dead)
        return this->respawnPosition;
    if(distance(this->position, this->map.pacman->getPosition()) < 8)
        return Position(0, this->map.getHeight());
    else
        return this->map.pacman->getPosition();
}

bool Clyde::mayLeave() const
{
    if(this->wasDead)
    {
        return this->timer.getElapsedTime().asMilliseconds() > 1000;
    }
    if(this->timer.getElapsedTime().asMilliseconds() >= 4000
        && (this->map.timer.getElapsedTime().asMilliseconds() >= 4000 || this->map.dotCounter.getCount() >= 30))
    {
        this->map.timer.restart();
        this->map.dotCounter.restart();
        std::cerr << "Clyde" << std::endl;
        return true;
    }
    return false;
}
