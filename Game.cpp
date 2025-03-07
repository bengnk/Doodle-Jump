#include "Game.hpp"
#include "Enemy.hpp"
#include "Boost.hpp"   // Neu: Boost einbinden
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Konstruktor: Initialisiert Fenster, Spieler, Score, Highscore, Zustände und lädt Schriftart.
Game::Game()
        : window(sf::VideoMode(400, 600), "Doodle Jump Clone"),
          player(sf::Vector2f(200, 500)),
          score(0),
          highScore(0),
          gameOver(false),
          paused(false)
{
    window.setFramerateLimit(60);
    // Schriftart laden – stelle sicher, dass "arial.ttf" im Arbeitsverzeichnis liegt.
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    init();
}

// init() setzt den Spielzustand zurück: löscht alte Plattformen, Gegner und Boosts,
// erzeugt neue Plattformen und setzt die Kameras und Spielerposition zurück.
void Game::init() {
    platforms.clear();
    enemies.clear();
    boosts.clear(); // Alle vorhandenen Boosts entfernen
    platforms.push_back(Platform(sf::Vector2f(150, 550)));
    srand(static_cast<unsigned>(time(0)));
    float yPos = 500;
    for (int i = 0; i < 10; ++i) {
        yPos -= 50 + (rand() % 50);
        float x = rand() % (window.getSize().x - 100);
        platforms.push_back(Platform(sf::Vector2f(x, yPos)));
    }
    // Kein initialer Gegner oder Boost, um direkte Kollisionen beim Start zu vermeiden.

    view = window.getDefaultView();
    hudView = window.getDefaultView();
    player.setPosition(sf::Vector2f(200, 500));
    score = 0;
    gameOver = false;
    paused = false;
}

// reset() aktualisiert den Highscore (falls nötig) und setzt das Spiel zurück.
void Game::reset() {
    if (score > highScore)
        highScore = score;
    init();
}

// run() enthält die Hauptspielschleife: verarbeitet Eingaben, aktualisiert und zeichnet den Spielzustand.
void Game::run() {
    sf::Clock clock;
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        processEvents();
        if (!gameOver && !paused) {
            update(deltaTime);
        }
        render();
    }
}

// processEvents() verarbeitet Fensterereignisse, schaltet bei ESC den Pausenstatus um,
// und bei Game Over kann mit R ein Reset erfolgen.
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (!gameOver && event.type == sf::Event::KeyPressed) {
            // ESC-Taste zum Pausieren oder Fortsetzen
            if (event.key.code == sf::Keyboard::Escape) {
                paused = !paused;
            }
        }
        if (gameOver && event.type == sf::Event::KeyPressed) {
            // Bei Game Over: Taste R zum Neustarten
            if (event.key.code == sf::Keyboard::R) {
                reset();
            }
        }
    }
}

// update() aktualisiert Spieler, Plattformen, Gegner und Boosts sowie Kollisionsprüfungen
// und die dynamische Generierung von Plattformen, Gegnern und Boosts.
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

    // Update aller Boosts (aktuell statisch, aber update()-Aufruf falls benötigt)
    for (auto& boost : boosts) {
        boost.update(deltaTime, window.getSize().x);
    }

    // Kollisionsprüfung zwischen Spieler und Plattformen anhand der unteren Spieler-Kante:
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

    // Kollisionsprüfung zwischen Spieler und Boosts:
    // Wird ein Boost eingesammelt, erhält der Spieler einen stärkeren Sprung (Boost) und der Boost wird entfernt.
    for (size_t i = 0; i < boosts.size(); i++) {
        if (playerBounds.intersects(boosts[i].getGlobalBounds())) {
            player.boost();
            boosts.erase(boosts.begin() + i);
            i--;
            break;
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

    // Kollisionsprüfung mit Gegnern – bei Berührung eines Gegners Game Over.
    for (auto& enemy : enemies) {
        if (playerBounds.intersects(enemy.getGlobalBounds())) {
            std::cout << "Game Over! Du hast einen Gegner berührt. Dein Score: " << score << std::endl;
            gameOver = true;
        }
    }

    // Kamera-Update: Wenn der Spieler höher springt als das aktuelle Kamerazentrum, wird die Kamera nach oben verschoben.
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

    // Dynamische Gegner-Generierung: erst ab Score > 100 und mit 0,5% Wahrscheinlichkeit pro Frame
    if (score > 100) {
        if (rand() % 1000 < 5) {
            float enemyX = rand() % (window.getSize().x - 40);
            float enemyY = topOfView - (rand() % 150 + 50);
            if (enemyY < player.getPosition().y - 150) {
                enemies.push_back(Enemy(sf::Vector2f(enemyX, enemyY)));
            }
        }
    }

    // Dynamische Boost-Generierung: Ab einem Score von 50 erscheint mit geringer Wahrscheinlichkeit ein Boost.
    if (score > 50) {
        if (rand() % 1000 < 3) {  // ca. 0,3% Wahrscheinlichkeit pro Frame
            float boostX = rand() % (window.getSize().x - 20);
            float boostY = topOfView - (rand() % 100 + 50);
            // Stelle sicher, dass der Boost mindestens 100 Pixel über dem Spieler spawnt.
            if (boostY < player.getPosition().y - 100) {
                boosts.push_back(Boost(sf::Vector2f(boostX, boostY)));
            }
        }
    }

    // Entferne Plattformen, die weit unterhalb des Sichtbereichs liegen.
    platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), [this](const Platform& p) {
                return p.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            platforms.end()
    );

    // Entferne Gegner, die weit unterhalb des Sichtbereichs liegen.
    enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [this](const Enemy& e) {
                return e.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            enemies.end()
    );

    // Entferne Boosts, die weit unterhalb des Sichtbereichs liegen.
    boosts.erase(
            std::remove_if(boosts.begin(), boosts.end(), [this](const Boost& b) {
                return b.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            boosts.end()
    );

    // Game Over-Bedingung: Fällt der Spieler zu weit unterhalb des Sichtbereichs, wird das Spiel beendet.
    if (player.getPosition().y > view.getCenter().y + 400 || player.getPosition().y > window.getSize().y) {
        std::cout << "Game Over! Dein Score: " << score << std::endl;
        gameOver = true;
    }

    // Aktualisiere den Score-Text (zeigt aktuellen Score und Highscore).
    scoreText.setString("Score: " + std::to_string(score) + "\nHighscore: " + std::to_string(highScore));
}

// render() zeichnet zuerst alle Spielobjekte (Plattformen, Gegner, Boosts, Spieler) in der scrollenden Game-View
// und danach die HUD-Elemente (Score, Pausen- oder Game Over-Anzeige) in der fixierten HUD-View.
void Game::render() {
    window.clear(sf::Color::Black);

    // Zeichne die Game-View (mit allen aktiven Objekten)
    window.setView(view);
    for (auto& platform : platforms) {
        platform.draw(window);
    }
    for (auto& enemy : enemies) {
        enemy.draw(window);
    }
    for (auto& boost : boosts) {
        boost.draw(window);
    }
    player.draw(window);

    // Setze die HUD-View, damit HUD-Elemente (Score, Pausen-/Game Over-Anzeige) fixiert bleiben.
    window.setView(hudView);
    // Im normalen Spiel werden nur Score-Text angezeigt.
    if (!gameOver && !paused) {
        window.draw(scoreText);
    }
    // Falls pausiert, zeige eine Pausenanzeige.
    if (paused && !gameOver) {
        sf::RectangleShape pauseBg;
        pauseBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        pauseBg.setFillColor(sf::Color(0, 0, 0, 150));
        pauseBg.setPosition(0, 0);
        window.draw(pauseBg);

        sf::Text pauseText;
        pauseText.setFont(font);
        pauseText.setCharacterSize(30);
        pauseText.setFillColor(sf::Color::Yellow);
        pauseText.setString("Paused\nPress ESC to Resume");
        sf::FloatRect textRect = pauseText.getLocalBounds();
        pauseText.setOrigin(textRect.left + textRect.width / 2.0f,
                            textRect.top + textRect.height / 2.0f);
        pauseText.setPosition(hudView.getCenter());
        window.draw(pauseText);
    }
    // Falls Game Over, wird eine Überlagerung mit Game Over-Text angezeigt.
    if (gameOver) {
        sf::RectangleShape gameOverBg;
        gameOverBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        gameOverBg.setFillColor(sf::Color(0, 0, 0, 200));
        gameOverBg.setPosition(0, 0);
        window.draw(gameOverBg);

        sf::Text gameOverText;
        gameOverText.setFont(font);
        gameOverText.setCharacterSize(30);
        gameOverText.setFillColor(sf::Color::Red);
        gameOverText.setString("Game Over!\nScore: " + std::to_string(score) +
                               "\nHighscore: " + std::to_string(highScore) +
                               "\nPress R to Restart");
        sf::FloatRect textRect = gameOverText.getLocalBounds();
        gameOverText.setOrigin(textRect.left + textRect.width / 2.0f,
                               textRect.top + textRect.height / 2.0f);
        gameOverText.setPosition(hudView.getCenter());
        window.draw(gameOverText);
    }

    window.display();
}
