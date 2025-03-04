#include "Player.hpp"
#include <SFML/Graphics.hpp>

Player::Player(const sf::Vector2f& startPos)
        : velocity(0.f, 0.f), gravity(500.f), jumpSpeed(-400.f)
{
    shape.setSize(sf::Vector2f(30, 30));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(startPos);
}

void Player::update(sf::Time deltaTime) {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shape.move(-200 * deltaTime.asSeconds(), 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shape.move(200 * deltaTime.asSeconds(), 0);
    }
    velocity.y += gravity * deltaTime.asSeconds();
    shape.move(0, velocity.y * deltaTime.asSeconds());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

sf::Vector2f Player::getVelocity() const {
    return velocity;
}

void Player::jump() {
    velocity.y = jumpSpeed;
}

void Player::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
}
