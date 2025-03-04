#ifndef ENEMY_HPP
#define ENEMY_HPP

#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(const sf::Vector2f& startPos);
    void update(sf::Time deltaTime, unsigned windowWidth);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
};

#endif // ENEMY_HPP
