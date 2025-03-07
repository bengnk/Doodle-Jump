#include "Player.hpp"
#include <SFML/Graphics.hpp>

// Konstruktor: Initialisiert die Spielfigur mit Startposition, Größe, Farbe und physikalischen Parametern.
Player::Player(const sf::Vector2f& startPos)
        : velocity(0.f, 0.f), gravity(500.f), jumpSpeed(-400.f)
{
    // Setze die Größe des Rechtecks (Spielfigur) und ihre Farbe
    shape.setSize(sf::Vector2f(30, 30));
    shape.setFillColor(sf::Color::Green);
    shape.setPosition(startPos);
}

// update(): Aktualisiert die Spielfigur basierend auf Tasteneingaben und wendet Schwerkraft an.
void Player::update(sf::Time deltaTime) {
    // Horizontale Bewegung: Links und Rechts
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
        shape.move(-200 * deltaTime.asSeconds(), 0);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
        shape.move(200 * deltaTime.asSeconds(), 0);
    }
    // Schwerkraft hinzufügen: Erhöhe die y-Geschwindigkeit
    velocity.y += gravity * deltaTime.asSeconds();
    // Bewege die Spielfigur entsprechend der aktuellen Geschwindigkeit
    shape.move(0, velocity.y * deltaTime.asSeconds());
}

// draw(): Zeichnet die Spielfigur auf das übergebene Fenster.
void Player::draw(sf::RenderWindow& window) {
    window.draw(shape);
}

// getGlobalBounds(): Gibt den globalen Begrenzungsrahmen der Spielfigur zurück (für Kollisionen).
sf::FloatRect Player::getGlobalBounds() const {
    return shape.getGlobalBounds();
}

// getPosition(): Liefert die aktuelle Position der Spielfigur.
sf::Vector2f Player::getPosition() const {
    return shape.getPosition();
}

// getVelocity(): Liefert die aktuelle Geschwindigkeit der Spielfigur.
sf::Vector2f Player::getVelocity() const {
    return velocity;
}

// jump(): Setzt die y-Geschwindigkeit auf den Standard-Sprungwert.
void Player::jump() {
    velocity.y = jumpSpeed;
}

// boost(): Löst einen verstärkten Sprung (Boost) aus – 1,5-fach stärker als der normale Sprung.
void Player::boost() {
    velocity.y = jumpSpeed * 1.5f;
}

// setPosition(): Setzt die Position der Spielfigur.
void Player::setPosition(const sf::Vector2f& pos) {
    shape.setPosition(pos);
}
