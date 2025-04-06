#include "Platform.hpp"
#include <cstdlib>

Platform::Platform(const sf::Vector2f& startPos) {
    shape.setSize(sf::Vector2f(100, 20));
    // Zufallsentscheidung für Plattformtyp:
    int r = rand() % 100;
    if (r < 30) { // 30% Wahrscheinlichkeit: Verschwinde­dende Plattform
        type = PlatformType::Disappearing;
        shape.setFillColor(sf::Color::Red);
    } else if (r < 50) { // 20% Wahrscheinlichkeit: Bewegliche Plattform
        type = PlatformType::Moving;
        shape.setFillColor(sf::Color::Green);
        velocity = sf::Vector2f(100.f, 0.f);
    } else { // Rest: Normale Plattform
        type = PlatformType::Normal;
        shape.setFillColor(sf::Color::Blue);
    }
    shape.setPosition(startPos);
}
bool Platform::isMoving() const {
    return type == PlatformType::Moving;
}

void Platform::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Platform::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

bool Platform::isDisappearing() const {
    return type == PlatformType::Disappearing;
}

void Platform::update(sf::Time deltaTime, unsigned windowWidth) {
    if (type == PlatformType::Moving) {
        shape.move(velocity * deltaTime.asSeconds());
        // Begrenzung: Bounce an den Seiten
        if (shape.getPosition().x < 0) {
            shape.setPosition(0, shape.getPosition().y);
            velocity.x = -velocity.x;
        }
        if (shape.getPosition().x + shape.getSize().x > windowWidth) {
            shape.setPosition(windowWidth - shape.getSize().x, shape.getPosition().y);
            velocity.x = -velocity.x;
        }
    }
}
