// standard c++ lib
#include <fstream>
#include <iostream>
#include <string>
#include <cmath>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "json/json.h"
#include "headers/config.hpp"
#include "headers/tile.hpp"

Defines DEFINES;
Config CONFIG;
MapData mapData;

sf::Vector2f Directions[] =
{
    sf::Vector2f(0, 0),
    sf::Vector2f(0, -1),
    sf::Vector2f(0, 1),
    sf::Vector2f(-1, 0),
    sf::Vector2f(1, 0)
};

// Coords

// ctors
Coords::Coords()
{
    this->x = 0.;
    this->y = 0.;
}
Coords::Coords(float val)
{
    this->x = this->y = val;
}

Coords::Coords(float x, float y)
{
    this->x = x;
    this->y = y;
}
Coords::Coords(sf::Vector2f& v)
{
    this->x = v.x;
    this->y = v.y;
}

// operators
Coords& Coords::operator=(Coords rval)
{
    if(this == &rval)
        return *this;

    this->x = rval.x;
    this->y = rval.y;
    return *this;
}
bool Coords::operator==(Coords& rval)
{
    return this->x == rval.x && this->y == rval.y;
}
Coords Coords::operator+(Coords rval)
{
    return Coords(this->x + rval.x, this->y + rval.y);
}
Coords Coords::operator+(sf::Vector2f rval)
{
    return Coords(this->x + rval.x, this->y + rval.y);
}
Coords Coords::operator-(Coords rval)
{
    return Coords(this->x - rval.x, this->y - rval.y);
}
Coords& Coords::operator+=(Coords rval)
{
    this->x += rval.x;
    this->y += rval.y;
    return *this;
}
Coords& Coords::operator+=(sf::Vector2f rval)
{
    this->x += rval.x;
    this->y += rval.y;
    return *this;
}

std::ostream& operator<<(std::ostream& out, Coords rval)
{
    out << rval.x << " " << rval.y;
    return out;
}


// Position

// ctors
Position::Position()
{
    this->c = 0;
    this->r = 0;
}
Position::Position(int c, int r)
{
    this->c = c;
    this->r = r;
}

// operators
Position& Position::operator=(Position rval)
{
    if(this == &rval)
        return *this;
    
    this->c = rval.c;
    this->r = rval.r;
    return *this;
}
bool Position::operator==(Position& rval)
{
    return this->c == rval.c && this->r == rval.r;
}
Position Position::operator+(Position rval)
{
    return Position(this->c + rval.c, this->r + rval.r);
}
Position Position::operator+(sf::Vector2f rval)
{
    return Position(this->c + int(rval.x), this->r + int(rval.y));
}
Position Position::operator-(Position rval)
{
    return Position(this->c - rval.c, this->r - rval.r);
}
Position Position::operator-(sf::Vector2f rval)
{
    return Position(this->c - int(rval.x), this->r - int(rval.y));
}
Position Position::operator%(sf::Vector2i rval)
{
    return Position(this->c % rval.x, this->r % rval.y);
}
Position& Position::operator+=(sf::Vector2f rval)
{
    this->c += int(rval.x);
    this->r += int(rval.y);
    return *this;
}
std::ostream& operator<<(std::ostream& out, Position rval)
{
    out << rval.c << " " << rval.r;
    return out;
}


int distance(Position lhv, Position rhv)
{
    return int(sqrt((lhv.c - rhv.c) * (lhv.c - rhv.c) - (lhv.r - rhv.r) * (lhv.r - rhv.r)));
}


bool loadConfigFile()
{
    std::ifstream configFile("config.cfg");
    Json::Value config;
    configFile >> config;

    try
    {
        CONFIG.resolution.width = config["resolution"]["width"].asInt();
        CONFIG.resolution.height = config["resolution"]["height"].asInt();
        CONFIG.fullscreen = config["fullscreen"].asBool();
    }
    catch(std::exception e)
    {
        std::cerr << "An error occurred during config loading in function loadConfigFile(). Config file corrupted or not existing." << std::endl;
        return false;
    }

    configFile.close();
    return true;
}

bool loadTextures()
{
    try
    {
        CONFIG.dotTexture = new sf::Texture;
        CONFIG.dotTexture->loadFromFile("textures/dot.png");
        CONFIG.superDotTexture = new sf::Texture;
        CONFIG.superDotTexture->loadFromFile("textures/superdot.png");
        CONFIG.wallTexture = new sf::Texture;
        CONFIG.wallTexture->loadFromFile("textures/wall.png");
        CONFIG.pacmanTexture = new sf::Texture;
        CONFIG.pacmanTexture->loadFromFile("textures/pacman.png");
        CONFIG.blinkyTexture = new sf::Texture;
        CONFIG.blinkyTexture->loadFromFile("textures/blinky.png");
        CONFIG.pinkyTexture = new sf::Texture;
        CONFIG.pinkyTexture->loadFromFile("textures/pinky.png");
        CONFIG.inkyTexture = new sf::Texture;
        CONFIG.inkyTexture->loadFromFile("textures/inky.png");
        CONFIG.clydeTexture = new sf::Texture;
        CONFIG.clydeTexture->loadFromFile("textures/clyde.png");
    }
    catch(std::exception e)
    {
        std::cerr << "An error occurred during textures loading in function loadTextures()." << std::endl;
        return false;
    }

    return true;
}

bool loadMap(MapData& mapData)
{
    std::ifstream configFile("config.cfg");
    if(!configFile.is_open())
    {
        std::cerr << "Config file not available." << std::endl;
        return false;
    }
    Json::Value config;
    configFile >> config;

    std::string mapName;

    try
    {
        mapName = config["map"].asString();
    }
    catch(std::exception e)
    {
        std::cerr << "An error occurred during config loading in function loadMap(). Config file corrupted or not existing." << std::endl;
        return false;
    }

    configFile.close();

    std::ifstream mapFile("maps/" + mapName + ".pmmap");
    Json::Value map;
    mapFile >> map;

    try
    {
        // map size loading
        mapData.height = map["height"].asInt();
        mapData.width = map["width"].asInt();

        // calculating sizes in pixels
        float tileSize = CONFIG.resolution.width / mapData.width;
        if(tileSize > CONFIG.resolution.height / (mapData.height + 20. / 7.))
            tileSize = CONFIG.resolution.height / (mapData.height + 20. / 7.);

        DEFINES.TILE_SIZE = int(tileSize);

        // setting hud margin
        DEFINES.HUD_MARGIN = int(20. * tileSize / 7);

        // calculating side margin
        DEFINES.SIDE_MARGIN = (CONFIG.resolution.width - DEFINES.TILE_SIZE * mapData.width) / 2;

        // calculating top margin
        DEFINES.TOP_MARGIN = (CONFIG.resolution.height - DEFINES.HUD_MARGIN - DEFINES.TILE_SIZE * mapData.height) / 2;
        
        // map objects data loading
        mapData.pacman.coords.x = DEFINES.SIDE_MARGIN + map["pacman"]["x"].asFloat() * DEFINES.TILE_SIZE;
        mapData.pacman.coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + map["pacman"]["y"].asFloat() * DEFINES.TILE_SIZE;
        mapData.blinky.coords.x = DEFINES.SIDE_MARGIN + map["blinky"]["x"].asFloat() * DEFINES.TILE_SIZE;
        mapData.blinky.coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + map["blinky"]["y"].asFloat() * DEFINES.TILE_SIZE;
        mapData.inky.coords.x = DEFINES.SIDE_MARGIN + map["inky"]["x"].asFloat() * DEFINES.TILE_SIZE;
        mapData.inky.coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + map["inky"]["y"].asFloat() * DEFINES.TILE_SIZE;
        mapData.pinky.coords.x = DEFINES.SIDE_MARGIN + map["pinky"]["x"].asFloat() * DEFINES.TILE_SIZE;
        mapData.pinky.coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + map["pinky"]["y"].asFloat() * DEFINES.TILE_SIZE;
        mapData.clyde.coords.x = DEFINES.SIDE_MARGIN + map["clyde"]["x"].asFloat() * DEFINES.TILE_SIZE;
        mapData.clyde.coords.y = DEFINES.TOP_MARGIN + DEFINES.HUD_MARGIN + map["clyde"]["y"].asFloat() * DEFINES.TILE_SIZE;
    }
    catch(std::exception e)
    {
        std::cerr << "An error occurred during map loading in function loadMap(). Map file corrupted or not existing." << std::endl;
        return false;
    }
    try
    {
        // map layout loading
        mapData.tiles = new Tile**[mapData.width];
        mapData.map = new int*[mapData.width];
        mapData.dotCount = 0;

        const Json::Value mapStringArray = map["map"];
        for(int c = 0; c < mapData.width; c++)
        {
            mapData.tiles[c] = new Tile*[mapData.height];
            mapData.map[c] = new int[mapData.height];
            for(int r = 0; r < mapData.height; r++)
            {
                char tile = mapStringArray[r].asString()[c];
                Position position = {c, r};
                switch(tile)
                {
                case ' ':
                    mapData.tiles[c][r] = new Tile(position, TileContents::none);
                    mapData.map[c][r] = 1;
                    break;
                case 't':
                    mapData.tiles[c][r] = new Tile(position, TileContents::tunel);
                    mapData.map[c][r] = 1;
                    break;
                case '.':
                    mapData.tiles[c][r] = new Tile(position, TileContents::dot, CONFIG.dotTexture);
                    mapData.map[c][r] = true;
                    break;
                case '*':
                    mapData.tiles[c][r] = new Tile(position, TileContents::superDot, CONFIG.superDotTexture);
                    mapData.map[c][r] = 1;
                    break;
                case '#':
                    mapData.tiles[c][r] = new Tile(position, TileContents::wall, CONFIG.wallTexture);
                    mapData.map[c][r] = 0;
                    break;
                case 'o':
                    mapData.tiles[c][r] = new Tile(position, TileContents::none);
                    mapData.map[c][r] = 0;
                    break;
                case 'd':
                    mapData.tiles[c][r] = new Tile(position, TileContents::ghosthouseDoor, CONFIG.wallTexture);
                    mapData.map[c][r] = 1;
                    break;
                }
            }
        }
    }
    catch(std::exception e)
    {
        std::cerr << "An error occurred during map loading in function loadMap(). Map file corrupted or not existing." << std::endl;
        return false;
    }

    return true;
}
