#include <SFML/Graphics.hpp>
#include <memory>

#include "headers/settings.hpp"
#include "headers/config.hpp"

int SettingsEntry::selected = 0;
int Resolution::currentValue;

SettingsEntry** settingsEntries = new SettingsEntry*[3];

SettingsEntry::SettingsEntry(std::shared_ptr<sf::Font>& font, std::string& entryText, float x, float y)
{
    this->font = font;

    this->entryTextNonSelected = std::make_unique<sf::Text>(entryText, *font, 40);
    this->entryTextSelected = std::make_unique<sf::Text>(entryText, *font, 40);

    this->x = x;
    this->y = y;
    
    this->entryTextNonSelected->setFillColor(sf::Color::White);
    this->entryTextNonSelected->setPosition(x, y);

    this->entryTextSelected->setFillColor(sf::Color::White);
    this->entryTextSelected->setOutlineColor(sf::Color::Yellow);
    this->entryTextSelected->setOutlineThickness(5);
    this->entryTextSelected->setPosition(x, y);
}

void SettingsEntry::draw(sf::RenderWindow& window, bool selected)
{
    if(selected)
    {
        window.draw(*(this->entryTextSelected));
        window.draw(*(this->valueTextSelected));
    }
    else
    {
        window.draw(*(this->entryTextNonSelected));
        window.draw(*(this->valueTextNonSelected));
    }
}

Resolution::Resolution(std::shared_ptr<sf::Font>& font, std::string& entryText, float x, float y) : SettingsEntry(font, entryText, x, y)
{
    this->valueTextNonSelected = std::make_unique<sf::Text>(this->values[currentValue], *font, 40);
    this->valueTextSelected = std::make_unique<sf::Text>(this->values[currentValue], *font, 40);

    this->valueTextNonSelected->setFillColor(sf::Color::White);
    this->valueTextNonSelected->setPosition(x + this->entryTextNonSelected->getLocalBounds().width + 10, y);

    this->valueTextSelected->setFillColor(sf::Color::White);
    this->valueTextSelected->setOutlineColor(sf::Color::Yellow);
    this->valueTextSelected->setOutlineThickness(5);
    this->valueTextSelected->setPosition(x + this->entryTextSelected->getLocalBounds().width + 10, y);
}

void Resolution::nextValue()
{
    if(this->currentValue < 3)
    {
        this->currentValue++;
        this->valueTextNonSelected->setString(this->values[currentValue]);
        this->valueTextNonSelected->setPosition(x + this->entryTextNonSelected->getLocalBounds().width + 10, y);
    }
}

void Resolution::previousValue()
{
    if(this->currentValue > 0)
    {
        this->currentValue--;
        this->valueTextNonSelected->setString(this->values[currentValue]);
        this->valueTextNonSelected->setPosition(x + this->entryTextNonSelected->getLocalBounds().width + 10, y);
    }
}

void processSettingsEvents(sf::RenderWindow& window)
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
                break;
            }
        }
    }
}

void settings(sf::RenderWindow& window)
{
    while(window.isOpen())
    {
        window.clear();

        processSettingsEvents(window);

        window.display();
    }
}
