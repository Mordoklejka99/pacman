#include <fstream>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <memory>

#include "json/json.h"
#include "json/json-forwards.h"
#include "headers/config.hpp"
#include "headers/tile.hpp"
#include "headers/settings.hpp"

Defines DEFINES;
Config CONFIG;

struct resolution {int width; int height;} Resolutions[] = {{800, 600}, {1280, 720}, {1280, 1024}};

bool loadConfig()
{
    try
    {
        //loading config file
        std::ifstream configFile;
        configFile.open("settings.cfg");

        Json::Value cfg;
        configFile >> cfg;

        //loading data from config file
        CONFIG.resolution.width = cfg["resolution"].get("width", 800).asInt();
        CONFIG.resolution.height = cfg["resolution"].get("height", 600).asInt();
        CONFIG.framerate = cfg.get("framerate", 30).asFloat();
        CONFIG.fullscreen = cfg.get("fullscreen", false).asBool();
        CONFIG.map = cfg.get("map", "default.pmmap").asString();

        configFile.close();     //closing file

        return true;
    }
    catch(std::exception e)
    {
        std::cerr << "An error occured during config file loading." << std::endl;
        return false;
    }
}

bool loadMenu()
{
    try
    {
        CONFIG.menuData = std::make_shared<MenuData>();
        CONFIG.menuData->backgroundImage = std::make_shared<sf::Texture>();

        for(int i = 0; i < RESOLUTION_COUNT; i++)
        {
            if(CONFIG.resolution.width == Resolutions[i].width && CONFIG.resolution.height == Resolutions[i].height)
            {
                CONFIG.menuData->backgroundImage->loadFromFile("img/bkg-" + std::to_string(Resolutions[i].width) + 'x' + std::to_string(Resolutions[i].height) + ".png");
                Resolution::currentValue = i;
                break;
            }
        }

        CONFIG.menuData->background = std::make_shared<sf::Sprite>(*(CONFIG.menuData->backgroundImage));

        CONFIG.menuData->titleFont = std::make_shared<sf::Font>();
        CONFIG.menuData->titleFont->loadFromFile("fonts/pacfont.ttf");

        sf::Font font;
        font.loadFromFile("fonts/crackman.ttf");

        CONFIG.menuData->entryFont = std::make_shared<sf::Font>(font);

        return true;
    }
    catch(...)
    {
        std::cerr << "An error occured during menu loading" << std::endl;
        return false;
    }
}

bool loadMap()
{
    try
    {
        //loading map file
        std::ifstream mapFile;
        mapFile.open("maps/" + CONFIG.map + ".pmmap");

        Json::Value map;
        mapFile >> map;

        CONFIG.mapData = std::make_shared<MapData>();

        //loading map size in tiles
        CONFIG.mapData->width = map.get("width", 28).asInt();
        CONFIG.mapData->height = map.get("height", 31).asInt();

        //setting top margin
        DEFINES.MAP_MARGIN_TOP = 80;

        //calculating sizes in pixels
        float tileSize = CONFIG.resolution.width / CONFIG.mapData->width;
        if(CONFIG.mapData->height * tileSize > CONFIG.resolution.height - DEFINES.MAP_MARGIN_TOP)
            tileSize = (CONFIG.resolution.height - DEFINES.MAP_MARGIN_TOP) / CONFIG.mapData->height;

        DEFINES.TILE_SIZE = tileSize;
        DEFINES.PACMAN_SIZE = tileSize / 2;
        DEFINES.GHOST_SIZE = tileSize / 2;

        //calculation side margins
        DEFINES.MAP_MARGIN_SIDE = (CONFIG.resolution.width - DEFINES.TILE_SIZE * CONFIG.mapData->width) / 2;

        //loading map image
        CONFIG.mapData->backgroundImage = std::make_shared<sf::Texture>();
        CONFIG.mapData->backgroundImage->loadFromFile("maps/" + CONFIG.map + ".png");
        // CONFIG.mapData->backgroundImage->loadFromFile("maps/def.png");

        //setting background position and scale
        CONFIG.mapData->background = std::make_shared<sf::Sprite>(*(CONFIG.mapData->backgroundImage));
        CONFIG.mapData->background->setPosition(DEFINES.MAP_MARGIN_SIDE, DEFINES.MAP_MARGIN_TOP);
        CONFIG.mapData->background->setScale(CONFIG.mapData->width * DEFINES.TILE_SIZE / CONFIG.mapData->backgroundImage->getSize().x, CONFIG.mapData->height * DEFINES.TILE_SIZE / CONFIG.mapData->backgroundImage->getSize().y);

        //loading dot and superdot image
        std::shared_ptr<sf::Texture> dotImage = std::make_shared<sf::Texture>();
        std::shared_ptr<sf::Texture> superDotImage = std::make_shared<sf::Texture>();
        dotImage->loadFromFile("img/dot.png");
        superDotImage->loadFromFile("img/superdot.png");

        //loading tiles from map file
        CONFIG.mapData->tiles = new Tile**[CONFIG.mapData->width];

        const Json::Value mapStringArray = map["map"];
        for(int c = 0; c < CONFIG.mapData->width; c++)
        {
            CONFIG.mapData->tiles[c] = new Tile*[CONFIG.mapData->height];
            for(int r = 0; r < CONFIG.mapData->height; r++)
            {
                char tile = mapStringArray[r].asString()[c];
                switch(tile)
                {
                case '#':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::wall);
                    break;
                case 'g':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::ghosthouse);
                    break;
                case 'd':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::ghosthouseDoor);
                    break;
                case '.':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::empty, Contents::dot);
                    break;
                case '*':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::empty, Contents::superdot);
                    break;
                case 't':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::tunel, Contents::none);
                    break;
                case ' ':
                    CONFIG.mapData->tiles[c][r] = new Tile(c, r, dotImage, superDotImage, Type::empty, Contents::none);
                    break;
                }
            }
        }

        CONFIG.mapData->pacmanX = DEFINES.MAP_MARGIN_SIDE + map["pacman"].get("x", 0).asFloat() * DEFINES.TILE_SIZE;
        CONFIG.mapData->pacmanY = DEFINES.MAP_MARGIN_TOP + map["pacman"].get("y", 0).asFloat() * DEFINES.TILE_SIZE;

        //loading ghost images;
        float scaleX;
        float scaleY;

        //Inky
        CONFIG.mapData->inky.x = DEFINES.MAP_MARGIN_SIDE + map["inky"].get("x", 0).asFloat() * DEFINES.TILE_SIZE;
        CONFIG.mapData->inky.y = DEFINES.MAP_MARGIN_TOP + map["inky"].get("y", 0).asFloat() * DEFINES.TILE_SIZE;

        CONFIG.mapData->inky.ghostImage = std::make_shared<sf::Texture>();
        CONFIG.mapData->inky.ghostImage->loadFromFile("img/inky.png");
        CONFIG.mapData->inky.ghostSprite = std::make_shared<sf::Sprite>(*CONFIG.mapData->inky.ghostImage);

        scaleX = DEFINES.TILE_SIZE / CONFIG.mapData->inky.ghostImage->getSize().x;
        scaleY = DEFINES.TILE_SIZE / CONFIG.mapData->inky.ghostImage->getSize().y;

        CONFIG.mapData->inky.ghostSprite->setScale(scaleX, scaleY);
        //Blinky
        CONFIG.mapData->blinky.x = DEFINES.MAP_MARGIN_SIDE + map["blinky"].get("x", 0).asFloat() * DEFINES.TILE_SIZE;
        CONFIG.mapData->blinky.y = DEFINES.MAP_MARGIN_TOP + map["blinky"].get("y", 0).asFloat() * DEFINES.TILE_SIZE;
        
        CONFIG.mapData->blinky.ghostImage = std::make_shared<sf::Texture>();
        CONFIG.mapData->blinky.ghostImage->loadFromFile("img/blinky.png");
        CONFIG.mapData->blinky.ghostSprite = std::make_shared<sf::Sprite>(*CONFIG.mapData->blinky.ghostImage);

        scaleX = DEFINES.TILE_SIZE / CONFIG.mapData->blinky.ghostImage->getSize().x;
        scaleY = DEFINES.TILE_SIZE / CONFIG.mapData->blinky.ghostImage->getSize().y;
        
        CONFIG.mapData->blinky.ghostSprite->setScale(scaleX, scaleY);
        //Pinky
        CONFIG.mapData->pinky.x = DEFINES.MAP_MARGIN_SIDE + map["pinky"].get("x", 0).asFloat() * DEFINES.TILE_SIZE;
        CONFIG.mapData->pinky.y = DEFINES.MAP_MARGIN_TOP + map["pinky"].get("y", 0).asFloat() * DEFINES.TILE_SIZE;
        
        CONFIG.mapData->pinky.ghostImage = std::make_shared<sf::Texture>();
        CONFIG.mapData->pinky.ghostImage->loadFromFile("img/pinky.png");
        CONFIG.mapData->pinky.ghostSprite = std::make_shared<sf::Sprite>(*CONFIG.mapData->pinky.ghostImage);

        scaleX = DEFINES.TILE_SIZE / CONFIG.mapData->pinky.ghostImage->getSize().x;
        scaleY = DEFINES.TILE_SIZE / CONFIG.mapData->pinky.ghostImage->getSize().y;
        
        CONFIG.mapData->pinky.ghostSprite->setScale(scaleX, scaleY);
        //Clyde
        CONFIG.mapData->clyde.x = DEFINES.MAP_MARGIN_SIDE + map["clyde"].get("x", 0).asFloat() * DEFINES.TILE_SIZE;
        CONFIG.mapData->clyde.y = DEFINES.MAP_MARGIN_TOP + map["clyde"].get("y", 0).asFloat() * DEFINES.TILE_SIZE;
        
        CONFIG.mapData->clyde.ghostImage = std::make_shared<sf::Texture>();
        CONFIG.mapData->clyde.ghostImage->loadFromFile("img/clyde.png");
        CONFIG.mapData->clyde.ghostSprite = std::make_shared<sf::Sprite>(*CONFIG.mapData->clyde.ghostImage);

        scaleX = DEFINES.TILE_SIZE / CONFIG.mapData->clyde.ghostImage->getSize().x;
        scaleY = DEFINES.TILE_SIZE / CONFIG.mapData->clyde.ghostImage->getSize().y;
        
        CONFIG.mapData->clyde.ghostSprite->setScale(scaleX, scaleY);
        
        return true;
    }
    catch(...)
    {
        std::cerr << "An error occured during map loading." << std::endl;
        return false;
    }
}

