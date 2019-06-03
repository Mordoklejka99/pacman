#include <iostream>
#include <memory>
#include <SFML/Graphics.hpp>

#include "headers/config.hpp"
#include "headers/ghost.hpp"

Ghost::Ghost(float x, float y, float speed, std::shared_ptr<sf::Texture>& ghostImage, std::shared_ptr<sf::Sprite>& ghostSprite, std::shared_ptr<Map>& map)
{
    this->x = x;
    this->y = y;
    this->speed = speed;
    this->ghostImage = ghostImage;
    this->ghostSprite = ghostSprite;
    this->map = map;

    this->ghostSprite->setPosition(this->x, this->y);
}


void Ghost::draw(sf::RenderWindow& window)
{
    window.draw(*this->ghostSprite);
}
