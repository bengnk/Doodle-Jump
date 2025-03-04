#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"  // Hinzugefügt, um die Enemy-Klasse zu kennen

class Game {
public:
    Game();
    void run();

private:
    void init();
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow window;
    Player player;
    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;  // Neu: Vektor für Gegner
    sf::View view;
    sf::View hudView;
    int score;
    sf::Font font;
    sf::Text scoreText;
};

#endif // GAME_HPP