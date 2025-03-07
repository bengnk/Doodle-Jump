#include "Boost.hpp"
#include <SFML/Graphics.hpp>

// Konstruktor: Erstellt einen Boost als Kreis mit Radius 10 und einer leuchtenden Cyan-Farbe.
Boost::Boost(const sf::Vector2f& startPos) {
    shape.setRadius(10.f);
    shape.setFillColor(sf::Color(0, 255, 255)); // Leuchtendes Cyan
    shape.setPosition(startPos);
}

// update(): Aktuell keine Bewegung – Funktion bleibt statisch, kann aber erweitert werden.
void Boost::update(sf::Time deltaTime, unsigned windowWidth) {
    // Für zukünftige Erweiterungen: Bewegung oder Animation des Boosts.
}

// draw(): Zeichnet den Boost auf das gegebene Fenster.
void Boost::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// getGlobalBounds(): Liefert den Begrenzungsrahmen des Boosts für Kollisionsprüfungen.
sf::FloatRect Boost::getGlobalBounds() const {
    return shape.getGlobalBounds();
}
