#include "Game.hpp"
#include "Enemy.hpp"
#include "Boost.hpp"   // Boost-Klasse einbinden
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>     // Für Dateioperationen

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

    // Konfiguriere den Score-Text: Schrift, Größe, Farbe und Position.
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    // Hintergrundbild laden und konfigurieren – stelle sicher, dass "background.png" im Arbeitsverzeichnis liegt.
    if (!backgroundTexture.loadFromFile("background.png")) {
        std::cerr << "Fehler beim Laden des Hintergrundbilds!" << std::endl;
    }
    backgroundSprite.setTexture(backgroundTexture);
    // Skalierung an die Fenstergröße anpassen:
    sf::Vector2u textureSize = backgroundTexture.getSize();
    sf::Vector2u windowSize = window.getSize();
    backgroundSprite.setScale(
            static_cast<float>(windowSize.x) / textureSize.x,
            static_cast<float>(windowSize.y) / textureSize.y
    );

    // Highscore aus Datei laden.
    loadHighScore();

    init();
}

void Game::loadHighScore() {
    std::ifstream file("highscore.txt");
    if (file.is_open()) {
        file >> highScore;
        file.close();
    } else {
        // Falls die Datei nicht existiert, wird highScore auf 0 belassen.
        highScore = 0;
    }
}

void Game::saveHighScore() {
    std::ofstream file("highscore.txt");
    if (file.is_open()) {
        file << highScore;
        file.close();
    } else {
        std::cerr << "Fehler beim Speichern des Highscores!" << std::endl;
    }
}

void Game::init() {
    platforms.clear();
    enemies.clear();
    boosts.clear(); // Vorhandene Boosts entfernen

    // Erstelle eine Basisplattform.
    platforms.push_back(Platform(sf::Vector2f(150, 550)));

    // Initialisiere den Zufallsgenerator.
    srand(static_cast<unsigned>(time(0)));

    // Erzeuge 10 weitere Plattformen in zufälligen vertikalen Abständen.
    float yPos = 500;
    for (int i = 0; i < 10; ++i) {
        yPos -= 50 + (rand() % 50);  // Abstand zwischen 50 und 99 Pixel
        float x = rand() % (window.getSize().x - 100);  // Plattformbreite = 100 Pixel
        platforms.push_back(Platform(sf::Vector2f(x, yPos)));
    }

    // Kein initialer Gegner oder Boost, um direkte Kollisionen beim Start zu vermeiden.

    // Setze die Ansichten (Kamera) auf die Standardansicht.
    view = window.getDefaultView();
    hudView = window.getDefaultView();

    // Setze die Startposition des Spielers und initialisiere Score und Zustände.
    player.setPosition(sf::Vector2f(200, 500));
    score = 0;
    gameOver = false;
    paused = false;
}

void Game::reset() {
    // Highscore aktualisieren und speichern, wenn der aktuelle Score höher ist.
    if (score > highScore) {
        highScore = score;
        saveHighScore();
    }
    init();
}

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

void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed)
            window.close();
        if (!gameOver && event.type == sf::Event::KeyPressed) {
            // ESC-Taste: Schaltet den Pausenstatus um.
            if (event.key.code == sf::Keyboard::Escape) {
                paused = !paused;
            }
        }
        if (gameOver && event.type == sf::Event::KeyPressed) {
            // Im Game Over-Zustand: Taste R startet das Spiel neu.
            if (event.key.code == sf::Keyboard::R) {
                reset();
            }
        }
    }
}

void Game::update(sf::Time deltaTime) {
    // Update des Spielers.
    player.update(deltaTime);

    // Update aller Plattformen (z. B. bewegliche Plattformen).
    for (auto& platform : platforms) {
        platform.update(deltaTime, window.getSize().x);
    }

    // Update aller Gegner.
    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window.getSize().x);
    }

    // Update aller Boosts.
    for (auto& boost : boosts) {
        boost.update(deltaTime, window.getSize().x);
    }

    // Kollisionsprüfung zwischen Spieler und Boosts:
    for (size_t i = 0; i < boosts.size(); i++) {
        if (player.getGlobalBounds().intersects(boosts[i].getGlobalBounds())) {
            player.boost();
            boosts.erase(boosts.begin() + i);
            i--;
            break;
        }
    }

    // Kollisionsprüfung: Spieler und Plattformen anhand der unteren Kante des Spielers.
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

    // Horizontales Wrap-Around:
    sf::Vector2f pos = player.getPosition();
    if (pos.x < 0) {
        pos.x = window.getSize().x;
        player.setPosition(pos);
    } else if (pos.x > window.getSize().x) {
        pos.x = 0;
        player.setPosition(pos);
    }

    // Kollisionsprüfung mit Gegnern:
    for (auto& enemy : enemies) {
        if (playerBounds.intersects(enemy.getGlobalBounds())) {
            std::cout << "Game Over! Du hast einen Gegner berührt. Dein Score: " << score << std::endl;
            gameOver = true;
        }
    }

    // Kamera-Update:
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

    // Dynamische Gegner-Generierung:
    if (score > 100) {
        if (rand() % 1000 < 2) {
            float enemyX = rand() % (window.getSize().x - 40);
            float enemyY = topOfView - (rand() % 150 + 50);
            if (enemyY < player.getPosition().y - 150) {
                enemies.push_back(Enemy(sf::Vector2f(enemyX, enemyY)));
            }
        }
    }

    // Dynamische Boost-Generierung:
    if (score > 50) {
        if (rand() % 1000 < 2) {
            std::vector<int> staticIndices;
            for (size_t i = 0; i < platforms.size(); i++) {
                if (!platforms[i].isMoving()) {
                    staticIndices.push_back(i);
                }
            }
            if (!staticIndices.empty()) {
                int idx = staticIndices[rand() % staticIndices.size()];
                sf::FloatRect platBounds = platforms[idx].getGlobalBounds();
                float boostX = platBounds.left + (platBounds.width / 2) - 10;
                float boostY = platBounds.top - 20;
                boosts.push_back(Boost(sf::Vector2f(boostX, boostY)));
            }
        }
    }

    // Entferne Plattformen, Gegner und Boosts, die weit unterhalb des Sichtbereichs liegen.
    platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), [this](const Platform& p) {
                return p.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            platforms.end()
    );
    enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [this](const Enemy& e) {
                return e.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            enemies.end()
    );
    boosts.erase(
            std::remove_if(boosts.begin(), boosts.end(), [this](const Boost& b) {
                return b.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            boosts.end()
    );

    // Game Over-Bedingung:
    if (player.getPosition().y > view.getCenter().y + 320 || player.getPosition().y > window.getSize().y) {
        std::cout << "Game Over! Dein Score: " << score << std::endl;
        gameOver = true;
    }

    // Aktualisiere den Score-Text, der den aktuellen Score und Highscore anzeigt.
    scoreText.setString("Score: " + std::to_string(score) + "\nHighscore: " + std::to_string(highScore));
}

void Game::render() {
    // Zuerst das Fenster mit der Hintergrundfarbe leeren.
    window.clear(sf::Color::Black);

    // Hintergrundbild zeichnen – hier verwenden wir die Default-Ansicht, damit der Hintergrund nicht mit der Kamera scrollt.
    window.setView(window.getDefaultView());
    window.draw(backgroundSprite);

    // Danach wieder auf die Spielansicht wechseln und alle aktiven Spielobjekte zeichnen.
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

    // HUD-Elemente zeichnen.
    window.setView(hudView);
    if (!gameOver && !paused) {
        window.draw(scoreText);
    }
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

    // Fensterinhalt anzeigen.
    window.display();
}
