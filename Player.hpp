#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

class Player {
public:
    Player(const sf::Vector2f& startPos);
    void update(sf::Time deltaTime);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
    sf::Vector2f getPosition() const;
    sf::Vector2f getVelocity() const;
    void jump();
    void boost(); // Neuer Boost: verleiht einen stärkeren Sprung
    void setPosition(const sf::Vector2f& pos);

private:
    sf::RectangleShape shape;
    sf::Vector2f velocity;
    const float gravity;
    const float jumpSpeed;
};

#endif // PLAYER_HPP
