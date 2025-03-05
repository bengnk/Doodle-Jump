#include "Game.hpp"
#include "Enemy.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

// Konstruktor: Initialisiert das Fenster, den Spieler, Score, Highscore sowie Zustände für Game Over und Pause.
// Lädt zudem die Schriftart und ruft init() auf, um den Spielzustand zu initialisieren.
Game::Game()
        : window(sf::VideoMode(400, 600), "Doodle Jump Clone"),
          player(sf::Vector2f(200, 500)),   // Setze die Startposition des Spielers
          score(0),
          highScore(0),
          gameOver(false),
          paused(false)
{
    // Setze die Bildwiederholrate (Framerate) auf 60 FPS
    window.setFramerateLimit(60);

    // Versuche, die Schriftart "arial.ttf" zu laden.
    // Stelle sicher, dass die Datei im Arbeitsverzeichnis liegt.
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Fehler beim Laden der Schriftart!" << std::endl;
    }

    // Konfiguriere den Score-Text: Schrift, Größe, Farbe und Position im Fenster.
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);

    // Initialisiere den Spielzustand (Plattformen, Kamera, etc.)
    init();
}

// init() setzt den Spielzustand zurück und erstellt anfängliche Plattformen (ohne Gegner-Spawns am Start).
void Game::init() {
    // Lösche alle vorhandenen Plattformen und Gegner (wichtig beim Reset)
    platforms.clear();
    enemies.clear();

    // Erstelle eine Basisplattform an einer festen Position.
    platforms.push_back(Platform(sf::Vector2f(150, 550)));

    // Initialisiere den Zufallsgenerator mit der aktuellen Zeit.
    srand(static_cast<unsigned>(time(0)));

    // Erzeuge 10 weitere Plattformen, die sich in der Höhe unterscheiden.
    // Die Plattformen werden in zufälligen Abständen (50 bis 99 Pixel) nach oben versetzt.
    float yPos = 500;
    for (int i = 0; i < 10; ++i) {
        yPos -= 50 + (rand() % 50);
        float x = rand() % (window.getSize().x - 100); // Plattformbreite ist 100 Pixel
        platforms.push_back(Platform(sf::Vector2f(x, yPos)));
    }

    // Es wird bewusst kein initialer Gegner erzeugt, um direkte Kollisionen beim Spielstart zu vermeiden.

    // Setze die Kameraperspektiven (Game-View und HUD-View) auf die Standardansicht des Fensters.
    view = window.getDefaultView();
    hudView = window.getDefaultView();

    // Setze die Startposition des Spielers und initialisiere Score sowie Zustände.
    player.setPosition(sf::Vector2f(200, 500));
    score = 0;
    gameOver = false;
    paused = false;
}

// reset() wird beim Neustart des Spiels (z. B. nach Game Over) aufgerufen.
// Aktualisiert den Highscore, falls der aktuelle Score höher ist, und initialisiert den Spielzustand neu.
void Game::reset() {
    if (score > highScore)
        highScore = score;
    init();
}

// run() enthält die Hauptspielschleife, die fortlaufend Eingaben verarbeitet, den Zustand aktualisiert und zeichnet.
void Game::run() {
    sf::Clock clock; // Clock zur Messung der verstrichenen Zeit pro Frame (deltaTime)
    while (window.isOpen()) {
        // deltaTime: Zeit, die seit dem letzten Frame vergangen ist.
        sf::Time deltaTime = clock.restart();
        processEvents();         // Verarbeite Eingaben (z. B. Tastatur, Mausklicks, Fenster schließen)
        if (!gameOver && !paused) {
            update(deltaTime);   // Aktualisiere den Spielzustand nur, wenn nicht Game Over und nicht pausiert
        }
        render();                // Zeichne den aktuellen Zustand
    }
}

// processEvents() verarbeitet alle Fensterereignisse, wie das Schließen des Fensters,
// das Pausieren (über ESC) und das Neustarten (über R) im Game Over-Zustand.
void Game::processEvents() {
    sf::Event event;
    while (window.pollEvent(event)) {
        // Fenster schließen
        if (event.type == sf::Event::Closed)
            window.close();

        // Wenn das Spiel aktiv ist (nicht Game Over), schalte den Pausenstatus mit ESC um.
        if (!gameOver && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Escape) {
                paused = !paused;
            }
        }
        // Wenn das Spiel im Game Over-Zustand ist, kann mit R ein Reset erfolgen.
        if (gameOver && event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::R) {
                reset();
            }
        }
    }
}

// update() aktualisiert den gesamten Spielzustand:
// Spieler, Plattformen, Gegner, Kollisionen, Kamera, dynamische Generierung neuer Objekte
void Game::update(sf::Time deltaTime) {
    // Aktualisiere den Spieler (Bewegung, Eingabe, Physik)
    player.update(deltaTime);

    // Aktualisiere alle Plattformen (zum Beispiel bewegliche Plattformen)
    for (auto& platform : platforms) {
        platform.update(deltaTime, window.getSize().x);
    }

    // Aktualisiere alle Gegner
    for (auto& enemy : enemies) {
        enemy.update(deltaTime, window.getSize().x);
    }

    // Kollisionsprüfung zwischen Spieler und Plattformen anhand der unteren Spieler-Kante:
    sf::FloatRect playerBounds = player.getGlobalBounds();
    float playerBottom = playerBounds.top + playerBounds.height;
    // Überprüfe jede Plattform
    for (size_t i = 0; i < platforms.size(); i++) {
        sf::FloatRect platBounds = platforms[i].getGlobalBounds();
        // Nur prüfen, wenn der Spieler nach unten fällt (positive y-Geschwindigkeit) und die Bounding-Boxen sich überschneiden.
        if (player.getVelocity().y > 0 && playerBounds.intersects(platBounds)) {
            // Prüfe, ob der Abstand zwischen der unteren Spielerkante und der oberen Plattformkante kleiner als 15 Pixel ist.
            if ((playerBottom - platBounds.top) >= 0 && (playerBottom - platBounds.top) < 15) {
                // Löst den Sprung des Spielers aus.
                player.jump();
                // Wenn es sich um eine "verschwindende" Plattform handelt, entferne sie.
                if (platforms[i].isDisappearing()) {
                    platforms.erase(platforms.begin() + i);
                    i--; // Passe den Index an, da ein Element gelöscht wurde.
                }
                break; // Verarbeite nur eine Kollision pro Frame.
            }
        }
    }

    // Horizontales Wrap-Around:
    // Falls der Spieler den linken Bildschirmrand verlässt, erscheint er rechts und umgekehrt.
    sf::Vector2f pos = player.getPosition();
    if (pos.x < 0) {
        pos.x = window.getSize().x;
        player.setPosition(pos);
    } else if (pos.x > window.getSize().x) {
        pos.x = 0;
        player.setPosition(pos);
    }

    // Kollisionsprüfung mit Gegnern:
    // Bei Berührung eines Gegners wird das Spiel beendet (Game Over).
    for (auto& enemy : enemies) {
        if (playerBounds.intersects(enemy.getGlobalBounds())) {
            std::cout << "Game Over! Du hast einen Gegner berührt. Dein Score: " << score << std::endl;
            gameOver = true;
        }
    }

    // Kamera-Update:
    // Wenn der Spieler höher springt als die aktuelle Kamerazentrierung, wird die Kamera nach oben verschoben.
    if (player.getPosition().y < view.getCenter().y) {
        view.setCenter(view.getCenter().x, player.getPosition().y);
        window.setView(view);
        // Aktualisiere den Score basierend auf der maximal erreichten Höhe.
        score = std::max(score, static_cast<int>(600 - player.getPosition().y));
    }

    // Dynamische Plattformgenerierung:
    // Berechne den oberen Rand der aktuellen Ansicht.
    float topOfView = view.getCenter().y - window.getSize().y / 2;
    // Finde die höchste (oberste) Plattform.
    float highestY = 1e9f;
    for (const auto& platform : platforms) {
        if (platform.getGlobalBounds().top < highestY)
            highestY = platform.getGlobalBounds().top;
    }
    // Solange der Abstand zwischen der obersten Plattform und dem oberen Rand der Ansicht größer als 120 Pixel ist,
    // werden neue Plattformen mit einem zufälligen vertikalen Abstand (zwischen 80 und 120 Pixel) generiert.
    while (highestY - topOfView > 120) {
        int offset = 80 + (rand() % 41);
        float newY = highestY - offset;
        float newX = rand() % (window.getSize().x - 100);
        platforms.push_back(Platform(sf::Vector2f(newX, newY)));
        highestY = newY;
    }

    // Dynamische Gegner-Generierung:
    // Ab einem Score von über 100 werden mit einer Wahrscheinlichkeit von 0,5% pro Frame Gegner generiert.
    // Es wird zusätzlich geprüft, dass der Gegner mindestens 150 Pixel über der Spielerposition spawnt,
    // um direkte Kollisionen zu vermeiden.
    if (score > 100) {
        if (rand() % 1000 < 5) {
            float enemyX = rand() % (window.getSize().x - 40);
            float enemyY = topOfView - (rand() % 150 + 50);
            if (enemyY < player.getPosition().y - 150) {
                enemies.push_back(Enemy(sf::Vector2f(enemyX, enemyY)));
            }
        }
    }

    // Entferne Plattformen, die weit unterhalb der aktuellen Ansicht liegen (außerhalb des Bildschirms).
    platforms.erase(
            std::remove_if(platforms.begin(), platforms.end(), [this](const Platform& p) {
                return p.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            platforms.end()
    );

    // Entferne Gegner, die weit unterhalb der aktuellen Ansicht liegen.
    enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(), [this](const Enemy& e) {
                return e.getGlobalBounds().top > view.getCenter().y + window.getSize().y;
            }),
            enemies.end()
    );

    // Game Over-Bedingung:
    // Wenn der Spieler zu weit unterhalb des Sichtbereichs fällt, wird das Spiel beendet.
    if (player.getPosition().y > view.getCenter().y + 400 || player.getPosition().y > window.getSize().y) {
        std::cout << "Game Over! Dein Score: " << score << std::endl;
        gameOver = true;
    }

    // Aktualisiere den Score-Text, der den aktuellen Score und Highscore anzeigt.
    scoreText.setString("Score: " + std::to_string(score) + "\nHighscore: " + std::to_string(highScore));
}

// render() zeichnet den aktuellen Spielzustand.
// Zuerst werden alle Spielobjekte in der Game-View gezeichnet, danach die HUD-Elemente in der fixierten HUD-View.
void Game::render() {
    // Leere den Fensterinhalt (mit schwarzem Hintergrund).
    window.clear(sf::Color::Black);

    // Setze die Game-View (Kamera), um die Spielobjekte (Plattformen, Gegner, Spieler) zu zeichnen.
    window.setView(view);
    for (auto& platform : platforms) {
        platform.draw(window);
    }
    for (auto& enemy : enemies) {
        enemy.draw(window);
    }
    player.draw(window);

    // Setze die HUD-View, damit die HUD-Elemente (Score, Pausen- oder Game Over-Anzeige) fixiert am Bildschirm bleiben.
    window.setView(hudView);
    // Wenn das Spiel aktiv ist (nicht Game Over und nicht pausiert), wird der Score-Text gezeichnet.
    if (!gameOver && !paused) {
        window.draw(scoreText);
    }
    // Wenn das Spiel pausiert ist (aber nicht Game Over), wird eine Pausenanzeige mit halbtransparentem Hintergrund angezeigt.
    if (paused && !gameOver) {
        sf::RectangleShape pauseBg;
        pauseBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        pauseBg.setFillColor(sf::Color(0, 0, 0, 150)); // Halbtransparenter schwarzer Hintergrund
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
    // Wenn das Spiel Game Over ist, wird eine vollständige Überlagerung mit Game Over-Text angezeigt.
    if (gameOver) {
        sf::RectangleShape gameOverBg;
        gameOverBg.setSize(sf::Vector2f(window.getSize().x, window.getSize().y));
        gameOverBg.setFillColor(sf::Color(0, 0, 0, 200)); // Dunkler, halbtransparenter Hintergrund
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

    // Zeige den gerenderten Frame an.
    window.display();
}
