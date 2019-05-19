#ifndef MENU_HPP
#define MENU_HPP

#include <memory>

class MenuEntry
{
    std::shared_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> textNonSelected;
    std::unique_ptr<sf::Text> textSelected;
    float y;
public:
    static int selected;

    MenuEntry(std::shared_ptr<sf::Font> font, std::string text, float y);
    virtual ~MenuEntry();

    void draw(sf::RenderWindow& window, bool selected = false);
    virtual void execute(sf::RenderWindow& window) = 0;
};

class NewGame : public MenuEntry
{
public:
    NewGame(std::shared_ptr<sf::Font> font, std::string text, float y) : MenuEntry(font, text, y) {};
    ~NewGame() {};
    virtual void execute(sf::RenderWindow& window) override;
};

class Settings : public MenuEntry
{
public:
    Settings(std::shared_ptr<sf::Font> font, std::string text, float y) : MenuEntry(font, text, y) {};
    ~Settings() {};
    virtual void execute(sf::RenderWindow& window) override;
};

class Exit : public MenuEntry
{
public:
    Exit(std::shared_ptr<sf::Font> font, std::string text, float y) : MenuEntry(font, text, y) {};
    ~Exit() {};
    virtual void execute(sf::RenderWindow& window) override;
};

int menu(sf::RenderWindow& window);

#endif
