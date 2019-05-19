#include <SFML/Graphics.hpp>
#include <string>
#include <iostream>
#include <memory>

#include "headers/menu.hpp"
#include "headers/game.hpp"
#include "headers/settings.hpp"
#include "headers/config.hpp"

int MenuEntry::selected = 0;
MenuEntry** menuEntries = new MenuEntry*[3];

MenuEntry::MenuEntry(std::shared_ptr<sf::Font> font, std::string text, float y)
{
    this->font = font;
    this->textNonSelected = std::make_unique<sf::Text>(text, *font, 40);
    this->textSelected = std::make_unique<sf::Text>(text, *font, 40);
    this->y = y;
    
    this->textNonSelected->setFillColor(sf::Color::White);
    this->textNonSelected->setPosition((CONFIG.resolution.width - this->textNonSelected->getLocalBounds().width) / 2, y);

    this->textSelected->setFillColor(sf::Color::White);
    this->textSelected->setOutlineColor(sf::Color::Yellow);
    this->textSelected->setOutlineThickness(5);
    this->textSelected->setPosition((CONFIG.resolution.width - this->textSelected->getLocalBounds().width) / 2, y);
}

MenuEntry::~MenuEntry()
{
    std::cerr << this->font.use_count() << std::endl;
}

void MenuEntry::draw(sf::RenderWindow& window, bool selected)
{
    if(selected)
        window.draw(*(this->textSelected));
    else
        window.draw(*(this->textNonSelected));
}

void NewGame::execute(sf::RenderWindow& window)
{
    game(window);
}

void Settings::execute(sf::RenderWindow& window)
{
    settings(window);
}

void Exit::execute(sf::RenderWindow& window)
{
    window.close();
}

void processMenuEvents(sf::RenderWindow& window)
{
    sf::Event event;
    while(window.pollEvent(event))
    {
        if(event.type == sf::Event::Closed)
        {
            window.close();
            return;
        }
        if(event.type == sf::Event::KeyPressed)
        {
            switch(event.key.code)
            {
            case sf::Keyboard::Escape:
                window.close();
                return;
            case sf::Keyboard::Up:
                if(MenuEntry::selected > 0)
                    MenuEntry::selected--;
                break;
            case sf::Keyboard::Down:
                if(MenuEntry::selected < 2)
                    MenuEntry::selected++;
                break;
            case sf::Keyboard::Return:
                menuEntries[MenuEntry::selected]->execute(window);
                break;
            }
        }
    }
}

int menu(sf::RenderWindow& window)
{
    if(loadMenu() == false)
        return 1;

    std::unique_ptr<sf::Text> title = std::make_unique<sf::Text>("PACMAN", *(CONFIG.menuData->titleFont), 96);
    title->setFillColor(sf::Color::White);
    title->setPosition((CONFIG.resolution.width - title->getLocalBounds().width) / 2, CONFIG.resolution.height / 8);

    menuEntries[0] = new NewGame(CONFIG.menuData->entryFont, "NEW GAME", CONFIG.resolution.height * 4 / 8);
    menuEntries[1] = new Settings(CONFIG.menuData->entryFont, "SETTINGS", CONFIG.resolution.height * 5 / 8);
    menuEntries[2] = new Exit(CONFIG.menuData->entryFont, "EXIT", CONFIG.resolution.height * 6 / 8);

    while(window.isOpen())
    {
        processMenuEvents(window);

        window.clear();

        window.draw(*(CONFIG.menuData->background));
        window.draw(*title);
        for(int i = 0; i < 3; i++)
            menuEntries[i]->draw(window, MenuEntry::selected == i ? true : false);
        
        window.display();
    }

    for(int i = 0; i < 3; i++)
        delete menuEntries[i];
    delete[] menuEntries;

    return 0;
}
