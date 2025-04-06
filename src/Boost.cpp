#include "Boost.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Boost::Boost(const sf::Vector2f& startPos) {
    // Setze den Radius des Boosts (z.B. 10 Pixel)
    shape.setRadius(10.f);
    // Setze eine leuchtende Farbe, z.B. Cyan
    shape.setFillColor(sf::Color(245, 20, 150));
    // Setze den Ursprung auf die Mitte, damit die Positionierung korrekt erfolgt.
    sf::FloatRect bounds = shape.getLocalBounds();
    shape.setOrigin(bounds.width / 2, bounds.height / 2);
    // Setze die Startposition des Boosts
    shape.setPosition(startPos);
    // Optional: Eine zusätzliche Skalierung, falls nötig, hier weggelassen.
}

void Boost::update(sf::Time deltaTime, unsigned windowWidth) {
    // Der Boost bewegt sich aktuell nicht.
}

void Boost::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

sf::FloatRect Boost::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
