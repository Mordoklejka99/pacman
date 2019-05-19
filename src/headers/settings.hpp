#ifndef SETTINGS_HPP
#define SETTINGS_HPP

#include <memory>

struct MenuData;

class SettingsEntry
{
protected:
    std::shared_ptr<sf::Font> font;
    std::unique_ptr<sf::Text> entryTextNonSelected;
    std::unique_ptr<sf::Text> entryTextSelected;
    std::unique_ptr<sf::Text> valueTextNonSelected;
    std::unique_ptr<sf::Text> valueTextSelected;
    float x;
    float y;
public:
    static int selected;

    SettingsEntry(std::shared_ptr<sf::Font>& font, std::string& entryText, float x, float y);
    virtual ~SettingsEntry() {};

    void draw(sf::RenderWindow& window, bool selected = false);
    virtual void nextValue() = 0;
    virtual void previousValue() = 0;
};

class Resolution : public SettingsEntry
{
    std::string values[3] = {"800x600", "1280x720", "1280x1024"};
    static int currentValue;
public:
    Resolution(std::shared_ptr<sf::Font>& font, std::string& entryText, float x, float y);
    ~Resolution() {};

    virtual void nextValue() override;
    virtual void previousValue() override;

    friend bool loadMenu();
};

void settings(sf::RenderWindow& window);

#endif
