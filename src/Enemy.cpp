#include "Enemy.hpp"
#include <cstdlib>

Enemy::Enemy(const sf::Vector2f& startPos) {
    shape.setSize(sf::Vector2f(40, 40));
    shape.setFillColor(sf::Color::Magenta);
    shape.setPosition(startPos);
    velocity = sf::Vector2f(80.f, 0.f);
}

void Enemy::update(sf::Time deltaTime, unsigned windowWidth) {
    shape.move(velocity * deltaTime.asSeconds());
    if (shape.getPosition().x < 0) {
        shape.setPosition(0, shape.getPosition().y);
        velocity.x = -velocity.x;
    } else if (shape.getPosition().x + shape.getSize().x > windowWidth) {
        shape.setPosition(windowWidth - shape.getSize().x, shape.getPosition().y);
        velocity.x = -velocity.x;
    }
}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Enemy::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
