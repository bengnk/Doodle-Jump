#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <SFML/Graphics.hpp>

// Die Player-Klasse repräsentiert den Spieler als Sprite mit zwei Texturen.
// Je nach Bewegungsrichtung wechselt der Sprite zwischen doodle_left.png und doodle_right.png.
class Player {
public:
    // Konstruktor: Lädt die Texturen, setzt den Sprite und initialisiert physikalische Parameter.
    Player(const sf::Vector2f& startPos);

    // Aktualisiert den Spieler: verarbeitet Eingaben, bewegt den Sprite und wendet Schwerkraft an.
    void update(sf::Time deltaTime);

    // Zeichnet den Spieler (Sprite) auf das übergebene Fenster.
    void draw(sf::RenderWindow& window);

    // Gibt den globalen Begrenzungsrahmen des Sprites zurück (für Kollisionsprüfungen).
    sf::FloatRect getGlobalBounds() const;

    // Liefert die aktuelle Position des Spielers.
    sf::Vector2f getPosition() const;

    // Liefert die aktuelle Geschwindigkeit des Spielers.
    sf::Vector2f getVelocity() const;

    // Löst einen normalen Sprung aus, indem die y-Geschwindigkeit auf jumpSpeed gesetzt wird.
    void jump();

    // Löst einen verstärkten Sprung (Boost) aus – 1,5-fach stärker.
    void boost();

    // Setzt die Position des Sprites.
    void setPosition(const sf::Vector2f& pos);

private:
    sf::Sprite sprite;          // Sprite, der den Spieler darstellt.
    sf::Texture leftTexture;    // Textur für den Spieler, wenn er nach links schaut.
    sf::Texture rightTexture;   // Textur für den Spieler, wenn er nach rechts schaut.
    bool facingRight;           // Gibt an, ob der Spieler momentan nach rechts schaut.

    sf::Vector2f velocity;      // Aktuelle Geschwindigkeit des Spielers.
    const float gravity;        // Konstante Schwerkraft.
    const float jumpSpeed;      // Standard-Sprunggeschwindigkeit.
};

#endif // PLAYER_HPP
