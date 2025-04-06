#ifndef BOOST_HPP
#define BOOST_HPP

#include <SFML/Graphics.hpp>

// Klasse Boost: Repräsentiert einen Power‑Up, den der Spieler einsammeln kann.
// Beim Einsammeln wird ein stärkerer Sprung ausgelöst.
class Boost {
public:
    Boost(const sf::Vector2f& startPos);
    void update(sf::Time deltaTime, unsigned windowWidth);
    void draw(sf::RenderWindow& window);
    sf::FloatRect getGlobalBounds() const;
private:
    sf::CircleShape shape;  // Statt Textur und Sprite wird ein CircleShape verwendet
};

#endif // BOOST_HPP
