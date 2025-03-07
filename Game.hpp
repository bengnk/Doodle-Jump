#ifndef GAME_HPP
#define GAME_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include "Player.hpp"
#include "Platform.hpp"
#include "Enemy.hpp"
#include "Boost.hpp" // Neu: Boost-Klasse einbinden

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
    std::vector<Boost> boosts;  // Neu: Vektor für Boosts
    sf::View view;
    sf::View hudView;
    int score;
    int highScore;
    bool gameOver;
    bool paused;
    sf::Font font;
    sf::Text scoreText;
};

#endif // GAME_HPP
