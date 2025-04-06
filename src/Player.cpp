#include "Player.hpp"
#include <SFML/Graphics.hpp>
#include <iostream>

Player::Player(const sf::Vector2f& startPos)
        : velocity(0.f, 0.f), gravity(500.f), jumpSpeed(-400.f)
{
    // Lade das Bild für den rechten Blick
    if (!rightTexture.loadFromFile("doodle_right.png")) {
        std::cerr << "Fehler beim Laden von doodle_right.png" << std::endl;
    }
    // Lade optional das Bild für den linken Blick (wird aber nicht direkt verwendet)
    if (!leftTexture.loadFromFile("doodle_left.png")) {
        std::cerr << "Fehler beim Laden von doodle_left.png" << std::endl;
    }
    // Verwende standardmäßig das rechte Bild.
    sprite.setTexture(rightTexture);
    // Setze den Ursprung zur Mitte, damit das Spiegeln korrekt funktioniert.
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    sprite.setPosition(startPos);
    // Verwende einen kleineren Skalierungsfaktor, damit der Charakter kleiner erscheint.
    const float scaleFactor = 0.05f;
    sprite.setScale(scaleFactor, scaleFactor);
}

void Player::update(sf::Time deltaTime) {
    bool movingLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
    bool movingRight = sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
    const float scaleFactor = 0.05f; // Gleicher Faktor wie im Konstruktor

    if (movingLeft) {
        // Spiegel den Sprite, falls er nicht bereits gespiegelt ist.
        if (sprite.getScale().x > 0) {
            sprite.setScale(-scaleFactor, scaleFactor);
        }
        sprite.move(-200 * deltaTime.asSeconds(), 0);
    } else if (movingRight) {
        // Setze den Sprite wieder in die normale Richtung.
        if (sprite.getScale().x < 0) {
            sprite.setScale(scaleFactor, scaleFactor);
        }
        sprite.move(200 * deltaTime.asSeconds(), 0);
    }
    // Schwerkraft anwenden.
    velocity.y += gravity * deltaTime.asSeconds();
    sprite.move(0, velocity.y * deltaTime.asSeconds());
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}

sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

sf::Vector2f Player::getVelocity() const {
    return velocity;
}

void Player::jump() {
    velocity.y = jumpSpeed;
}

void Player::boost() {
    velocity.y = jumpSpeed * 1.5f;
}

void Player::setPosition(const sf::Vector2f& pos) {
    sprite.setPosition(pos);
}
