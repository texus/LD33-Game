#ifndef GAME_HPP
#define GAME_HPP

#include <TGUI/TGUI.hpp>
#include <SFML/Audio.hpp>
#include <random>

extern std::random_device rd;
extern std::mt19937 generator;

enum class Direction
{
    Left,
    Right,
    Up,
    Down
};

struct Enemy
{
    sf::Vector2u square;
    sf::Vector2f pos;
    float speed;

    bool moving = false;
    sf::Vector2f moveFrom;
    sf::Vector2f moveTo;
    sf::Vector2i direction;
};

struct Player
{
    float life = 100;
    sf::Vector2u square;
    sf::Vector2f pos;

    bool moving = false;
    sf::Vector2f moveFrom;
    sf::Vector2f moveTo;
    Direction direction = Direction::Down;
};

class Game
{
public:
    Game();
    void loadResources();
    void showMenu();

private:
    void showHelpScreen();
    void showGameOverScreen(bool victory);
    void resetSettings();
    void setup();
    void play();

    void updateGame(float elapsedTime);
    void drawGame();

private:

    // Global
    const unsigned windowWidth = 800;
    const unsigned windowHeight = 600;

    sf::RenderWindow window;
    tgui::Gui gui;

    // Game settings
    unsigned mazeWidth;
    unsigned mazeHeight;
    float tileSize;
    float playerSpeed;

    float minEnemyWalkSpeed;
    float minEnemyRunSpeed;
    float maxEnemyWalkSpeed;
    float maxEnemyRunSpeed;

    unsigned orbsCount;
    unsigned enemyCount;
    float enemyHitDamage;
    float darknessLifeDrain;

    // Resources
    sf::Texture playerLightTexture;
    sf::Sprite playerLightSprite;

    sf::Texture enemyLightTexture;
    sf::Sprite enemyLightSprite;

    sf::Texture orbBackTexture;
    sf::Sprite orbBackSprite;

    sf::Texture orbFrontTexture;
    sf::Sprite orbFrontSprite;

    sf::Texture directionArrowTexture;
    sf::Sprite directionArrowSprite;

    unsigned int currentPlayerTexture;
    std::map<Direction, std::vector<sf::Texture>> playerTextures;
    sf::Sprite playerSprite;

    unsigned int currentEnemyTexture;
    std::map<Direction, std::vector<sf::Texture>> enemyTextures;
    sf::Sprite enemySprite;

    sf::SoundBuffer soundBufferHit;
    sf::SoundBuffer soundBufferPickup;
    sf::SoundBuffer soundBufferVictory;
    sf::SoundBuffer soundBufferGameOver;

    // Game variables
    std::list<sf::Sound> sounds;
    std::vector<std::vector<char>> maze;
    std::vector<sf::Vector2u> orbList;
    std::list<Enemy> enemies;
    Player player;

    sf::View view;
    float enemyWalkSpeed;
    float enemyRunSpeed;

    float lightTimer;
    float playerTimer;
    float orbTimer;
    bool orbTimerIncreasing;

    tgui::ProgressBar::Ptr healthBar;
};

#endif // GAME_HPP
