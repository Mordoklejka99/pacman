// standard c++ lib
#include <fstream>
#include <iostream>
#include <string>
#include <thread>

// additional libs
#include <SFML/Graphics.hpp>

// project headers
#include "json/json.h"
#include "headers/config.hpp"
#include "headers/tile.hpp"

Defines DEFINES;
Config CONFIG;
MapData mapData;

bool loadConfigFile()
{
    std::ifstream configFile("config.cfg");
    Json::Value config;
    configFile >> config;

    try
    {
        CONFIG.resolution.width = config["resolution"]["width"].asUInt();
        CONFIG.resolution.height = config["resolution"]["height"].asUInt();
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
        // map data loading
        mapData.height = map["height"].asUInt();
        mapData.width = map["width"].asUInt();
        mapData.pacman.x = map["pacman"]["x"].asUInt();
        mapData.pacman.y = map["pacman"]["x"].asUInt();
        mapData.inky.x = map["pacman"]["x"].asUInt();
        mapData.inky.y = map["pacman"]["x"].asUInt();
        mapData.blinky.x = map["pacman"]["x"].asUInt();
        mapData.blinky.y = map["pacman"]["x"].asUInt();
        mapData.pinky.x = map["pacman"]["x"].asUInt();
        mapData.pinky.y = map["pacman"]["x"].asUInt();
        mapData.clyde.x = map["pacman"]["x"].asUInt();
        mapData.clyde.y = map["pacman"]["x"].asUInt();

        // calculating sizes in pixels
        float tileSize = CONFIG.resolution.width / mapData.width;
        if(tileSize > CONFIG.resolution.height / (mapData.height + 40. / 7.))
            tileSize = CONFIG.resolution.height / (mapData.height + 40. / 7.);

        DEFINES.TILE_SIZE = int(tileSize);

        // setting hud margin
        DEFINES.HUD_MARGIN = int(40. * tileSize / 7);

        // calculating side margin
        DEFINES.SIDE_MARGIN = (CONFIG.resolution.width - DEFINES.TILE_SIZE * mapData.width) / 2;

        // calculating top margin
        DEFINES.TOP_MARGIN = (CONFIG.resolution.height - DEFINES.HUD_MARGIN - DEFINES.TILE_SIZE * mapData.height) / 2;
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
        mapData.map = new bool*[mapData.width];
        mapData.dotCount = 0;

        const Json::Value mapStringArray = map["map"];
        for(uint c = 0; c < mapData.width; c++)
        {
            mapData.tiles[c] = new Tile*[mapData.height];
            mapData.map[c] = new bool[mapData.height];
            for(uint r = 0; r < mapData.height; r++)
            {
                char tile = mapStringArray[r].asString()[c];
                Position position = {c, r};
                switch(tile)
                {
                case ' ':
                    mapData.tiles[c][r] = new Tile(position, TileContents::none);
                    mapData.map[c][r] = true;
                    break;
                case 't':
                    mapData.tiles[c][r] = new Tile(position, TileContents::tunel);
                    mapData.map[c][r] = true;
                    break;
                case '.':
                    mapData.tiles[c][r] = new Tile(position, TileContents::dot, CONFIG.dotTexture);
                    mapData.map[c][r] = true;
                    break;
                case '*':
                    mapData.tiles[c][r] = new Tile(position, TileContents::superDot, CONFIG.superDotTexture);
                    mapData.map[c][r] = true;
                    break;
                case '#':
                    mapData.tiles[c][r] = new Tile(position, TileContents::wall, CONFIG.wallTexture);
                    mapData.map[c][r] = false;
                    break;
                case 'd':
                    mapData.tiles[c][r] = new Tile(position, TileContents::ghosthouseDoor, CONFIG.wallTexture);
                    mapData.map[c][r] = false;
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
