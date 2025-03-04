#include "Game.hpp"
#include "Enemy.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game()
        : window(sf::VideoMode(400, 600), "Doodle Jump Clone"),
          player(sf::Vector2f(200, 500)),
          score(0)
{
    window.setFramerateLimit(60);
    // Schriftart laden (stelle sicher, dass "arial.ttf" im Arbeitsverzeichnis liegt)
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    init();
}

void Game::init() {
    platforms.push_back(Platform(sf::Vector2f(150, 550)));
    srand(static_cast<unsigned>(time(0)));
    float yPos = 500;
    for (int i = 0; i < 10; ++i) {
        yPos -= 50 + (rand() % 50);
        float x = rand() % (window.getSize().x - 100);
        platforms.push_back(Platform(sf::Vector2f(x, yPos)));
    }
    // Erstelle einen ersten Gegner (optional)
    enemies.push_back(Enemy(sf::Vector2f(200, 400)));

    view = window.getDefaultView();
    hudView = window.getDefaultView();
}

void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        update(deltaTime);
        render();
    }
}

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}

void Game::update(sf::Time deltaTime) {
    // Update des Spielers
    player.update(deltaTime);

    // Update aller Plattformen (z. B. bewegliche Plattformen)
    for (auto& platform : platforms) {
        platform.update(deltaTime, window.getSize().x);
    }

    // Update aller Gegner
    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window.getSize().x);
    }

    // Kollisionsprüfung anhand der unteren Kante des Spielers:
    sf::FloatRect playerBounds = player.getGlobalBounds();
    float playerBottom = playerBounds.top + playerBounds.height;
    for (size_t i = 0; i < platforms.size(); i++) {
        sf::FloatRect platBounds = platforms[i].getGlobalBounds();
        if (player.getVelocity().y > 0 && playerBounds.intersects(platBounds)) {
            if ((playerBottom - platBounds.top) >= 0 && (playerBottom - platBounds.top) < 15) {
                player.jump();
                if (platforms[i].isDisappearing()) {
                    platforms.erase(platforms.begin() + i);
                    i--;
                }
                break;
            }
        }
    }

    // Horizontales Wrap-Around für den Spieler:
    sf::Vector2f pos = player.getPosition();
    if (pos.x < 0) {
        pos.x = window.getSize().x;
        player.setPosition(pos);
    } else if (pos.x > window.getSize().x) {
        pos.x = 0;
        player.setPosition(pos);
    }

    // Kollisionsprüfung mit Gegnern – Berührt der Spieler einen Gegner, Game Over
    for (auto& enemy : enemies) {
        if (playerBounds.intersects(enemy.getGlobalBounds())) {
            std::cout << "Game Over! Du hast einen Gegner berührt. Dein Score: " << score << std::endl;
            window.close();
        }
    }

    // Kamera aktualisieren, wenn der Spieler höher springt
    if (player.getPosition().y < view.getCenter().y) {
        view.setCenter(view.getCenter().x, player.getPosition().y);
        window.setView(view);
        score = std::max(score, static_cast<int>(600 - player.getPosition().y));
    }

    // Dynamische Plattformgenerierung:
    float topOfView = view.getCenter().y - window.getSize().y / 2;
    float highestY = 1e9f;
    for (const auto& platform : platforms) {
        if (platform.getGlobalBounds().top < highestY)
            highestY = platform.getGlobalBounds().top;
    }
    while (highestY - topOfView > 120) {
        int offset = 80 + (rand() % 41);
        float newY = highestY - offset;
        float newX = rand() % (window.getSize().x - 100);
        platforms.push_back(Platform(sf::Vector2f(newX, newY)));
        highestY = newY;
    }

    // Dynamische Gegner-Generierung: Nur ab Score > 100 und mit geringerer Wahrscheinlichkeit (0,5% pro Frame)
    if (score > 100) {
        if (rand() % 1000 < 5) {  // 0.5% Chance pro Frame
            float enemyX = rand() % (window.getSize().x - 40);
            float enemyY = topOfView - (rand() % 150 + 50); // Spawn weiter oben
            enemies.push_back(Enemy(sf::Vector2f(enemyX, enemyY)));
        }
    }

    // Entferne Plattformen, die weit unterhalb des Sichtbereichs liegen
    platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), [this](const Platform& p) {
                return p.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            platforms.end()
    );

    // Entferne Gegner, die weit unterhalb des Sichtbereichs liegen
    enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [this](const Enemy& e) {
                return e.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            enemies.end()
    );

    // Game Over-Bedingung: Fällt der Spieler zu weit unterhalb des Sichtbereichs
    if (player.getPosition().y > view.getCenter().y + 400 || player.getPosition().y > window.getSize().y) {
        std::cout << "Game Over! Dein Score: " << score << std::endl;
        window.close();
    }

    // Score-Text aktualisieren
    scoreText.setString("Score: " + std::to_string(score));
}

void Game::render() {
    window.clear(sf::Color::Black);

    // Zeichne Spielobjekte in der Game-View
    window.setView(view);
    for (auto& platform : platforms) {
        platform.draw(window);
    }
    for (auto& enemy : enemies) {
        enemy.draw(window);
    }
    player.draw(window);

    // Zeichne HUD (Score) in der fixierten HUD-View
    window.setView(hudView);
    window.draw(scoreText);

    window.display();
}
