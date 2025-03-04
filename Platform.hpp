#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include <SFML/Graphics.hpp>

enum class PlatformType { Normal, Disappearing, Moving };

class Platform {
public:
    // Konstruktor: Setzt Startposition und bestimmt zufällig den Plattformtyp
    Platform(const sf::Vector2f& startPos);

    // Zeichnet die Plattform
    void draw(sf::RenderWindow& window);

    // Gibt die globale Begrenzung zurück (für Kollision)
    sf::FloatRect getGlobalBounds() const;

    // Gibt zurück, ob die Plattform verschwinden soll
    bool isDisappearing() const;

    // Update-Funktion für bewegliche Plattformen; windowWidth wird zur Begrenzung benötigt
    void update(sf::Time deltaTime, unsigned windowWidth);

private:
    sf::RectangleShape shape;
    PlatformType type;
    // Für Typ Moving: horizontale Geschwindigkeit
    sf::Vector2f velocity;
};

#endif // PLATFORM_HPP
