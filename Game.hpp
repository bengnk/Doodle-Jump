#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"

class Game {
public:
    Game();
    void run();
    void reset();

private:
    void init();
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    sf::RenderWindow window;
    Player player;
    std::vector<Platform> platforms;
    std::vector<Enemy> enemies;
    sf::View view;
    sf::View hudView;
    int score;
    int highScore;
    bool gameOver;
    bool paused;    // Neu: Variable zum Pausieren des Spiels
    sf::Font font;
    sf::Text scoreText;
};

#endif // GAME_HPP
