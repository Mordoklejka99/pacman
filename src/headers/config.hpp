#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <memory>

class Tile;

#define PACMAN_SPEED 3.0f
#define GHOST_SPEED 4.0f

#define RESOLUTION_COUNT 3

struct Defines
{
    float MAP_MARGIN_TOP;
    float MAP_MARGIN_SIDE;
    float PACMAN_SIZE;
    float GHOST_SIZE;
    float TILE_SIZE;
};

struct MenuData
{
    std::shared_ptr<sf::Texture> backgroundImage;
    std::shared_ptr<sf::Sprite> background;
    std::shared_ptr<sf::Font> titleFont;
    std::shared_ptr<sf::Font> entryFont;
};

struct MapData
{
    int width;
    int height;
    float pacmanX;
    float pacmanY;
    Tile*** tiles;
    std::shared_ptr<sf::Texture> backgroundImage;
    std::shared_ptr<sf::Sprite> background;

    struct Inky
    {
        float x;
        float y;
        std::shared_ptr<sf::Texture> ghostImage;
        std::shared_ptr<sf::Sprite> ghostSprite;
    } inky;
    struct Blinky
    {
        float x;
        float y;
        std::shared_ptr<sf::Texture> ghostImage;
        std::shared_ptr<sf::Sprite> ghostSprite;
    } blinky;
    struct Pinky
    {
        float x;
        float y;
        std::shared_ptr<sf::Texture> ghostImage;
        std::shared_ptr<sf::Sprite> ghostSprite;
    } pinky;
    struct Clyde
    {
        float x;
        float y;
        std::shared_ptr<sf::Texture> ghostImage;
        std::shared_ptr<sf::Sprite> ghostSprite;
    } clyde;
};

struct Config
{
    struct
    {
        int width;
        int height;
    } resolution;
    float framerate;
    bool fullscreen;
    std::string map;

    std::shared_ptr<MenuData> menuData;
    std::shared_ptr<MapData> mapData;
};

extern Defines DEFINES;
extern Config CONFIG;

bool loadConfig();
bool loadMenu();
bool loadMap();

#endif
