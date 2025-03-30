#include "Boost.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Boost::Boost(const sf::Vector2f& startPos) {
    if (!texture.loadFromFile("boost.png")) {
        std::cerr << "Fehler beim Laden von boost.png" << std::endl;
    }
    sprite.setTexture(texture);
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    sprite.setPosition(startPos);

    // Testweise: Temporärer größerer Skalierungsfaktor, z. B. 0.1f
    sprite.setScale(0.01f, 0.01f);
}

void Boost::update(sf::Time deltaTime, unsigned windowWidth) {
    // Der Boost bewegt sich aktuell nicht.
}

void Boost::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Boost::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}
