#ifndef BOOST_HPP
#define BOOST_HPP

#include <SFML/Graphics.hpp>

// Klasse Boost: Repräsentiert einen Power‑Up, den der Spieler einsammeln kann.
// Beim Einsammeln wird ein stärkerer Sprung ausgelöst.
class Boost {
public:
    // Konstruktor: Setzt die Startposition des Boosts
    Boost(const sf::Vector2f& startPos);

    // Update-Methode (hier statisch, kann später erweitert werden)
    void update(sf::Time deltaTime, unsigned windowWidth);

    // Zeichnet den Boost auf das Fenster
    void draw(sf::RenderWindow& window);

    // Gibt die globale Begrenzung des Boosts zurück (für Kollision)
    sf::FloatRect getGlobalBounds() const;

private:
    sf::CircleShape shape; // Darstellung als Kreis (z. B. goldgelb oder cyan)
};

#endif // BOOST_HPP
