#include "Game.hpp"
#include "MazeGenerator.hpp"

std::random_device rd;
std::mt19937 generator(rd());

Game::Game() :
    window{{windowWidth, windowHeight}, "LD33 Game"},
    gui{window}
{
    window.setFramerateLimit(60);
    resetSettings();
}

void Game::loadResources()
{
    gui.setGlobalFont("TGUI/fonts/DejaVuSans.ttf");

    playerLightTexture.loadFromFile("resources/PlayerLight.png");
    playerLightSprite.setTexture(playerLightTexture);

    orbBackTexture.loadFromFile("resources/OrbBack.png");
    orbBackSprite.setTexture(orbBackTexture);

    orbFrontTexture.loadFromFile("resources/OrbFront.png");
    orbFrontSprite.setTexture(orbFrontTexture);

    directionArrowTexture.loadFromFile("resources/DirectionArrow.png");
    directionArrowSprite.setTexture(directionArrowTexture);
    directionArrowSprite.setOrigin({directionArrowTexture.getSize().x / 2.f, directionArrowTexture.getSize().y / 2.f});
    directionArrowSprite.setPosition(20, 20);
    directionArrowSprite.setColor({0, 0, 255});

    playerTextures[Direction::Down].push_back({});
    playerTextures[Direction::Down].back().loadFromFile("resources/PlayerDown1.png");
    playerTextures[Direction::Down].push_back({});
    playerTextures[Direction::Down].back().loadFromFile("resources/PlayerDown2.png");
    playerTextures[Direction::Up].push_back({});
    playerTextures[Direction::Up].back().loadFromFile("resources/PlayerUp1.png");
    playerTextures[Direction::Up].push_back({});
    playerTextures[Direction::Up].back().loadFromFile("resources/PlayerUp2.png");
    playerTextures[Direction::Right].push_back({});
    playerTextures[Direction::Right].back().loadFromFile("resources/PlayerRight1.png");
    playerTextures[Direction::Right].push_back({});
    playerTextures[Direction::Right].back().loadFromFile("resources/PlayerRight2.png");
    playerTextures[Direction::Left].push_back({});
    playerTextures[Direction::Left].back().loadFromFile("resources/PlayerLeft1.png");
    playerTextures[Direction::Left].push_back({});
    playerTextures[Direction::Left].back().loadFromFile("resources/PlayerLeft2.png");

    enemyTextures[Direction::Down].push_back({});
    enemyTextures[Direction::Down].back().loadFromFile("resources/EnemyDown1.png");
    enemyTextures[Direction::Down].push_back({});
    enemyTextures[Direction::Down].back().loadFromFile("resources/EnemyDown2.png");
    enemyTextures[Direction::Up].push_back({});
    enemyTextures[Direction::Up].back().loadFromFile("resources/EnemyUp1.png");
    enemyTextures[Direction::Up].push_back({});
    enemyTextures[Direction::Up].back().loadFromFile("resources/EnemyUp2.png");
    enemyTextures[Direction::Right].push_back({});
    enemyTextures[Direction::Right].back().loadFromFile("resources/EnemyRight1.png");
    enemyTextures[Direction::Right].push_back({});
    enemyTextures[Direction::Right].back().loadFromFile("resources/EnemyRight2.png");
    enemyTextures[Direction::Left].push_back({});
    enemyTextures[Direction::Left].back().loadFromFile("resources/EnemyLeft1.png");
    enemyTextures[Direction::Left].push_back({});
    enemyTextures[Direction::Left].back().loadFromFile("resources/EnemyLeft2.png");

    soundBufferHit.loadFromFile("resources/Hit.wav");
    soundBufferPickup.loadFromFile("resources/Pickup.wav");
    soundBufferVictory.loadFromFile("resources/Victory.wav");
    soundBufferGameOver.loadFromFile("resources/GameOver.wav");
}

void Game::showMenu()
{
    while (window.isOpen())
    {
        gui.removeAllWidgets();
        auto theme = std::make_shared<tgui::Theme>("TGUI/widgets/Black.txt");

        tgui::Label::Ptr label = std::make_shared<tgui::Label>();
        label->setTextSize(20);
        label->setPosition(50, 40);
        label->setText("Maze:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 70);
        label->setText("Width: (10-100)");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(275, 70);
        label->setText("Height: (10-100)");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(475, 70);
        label->setText("Tile size:");
        gui.add(label);

        tgui::EditBox::Ptr mazeWidthEditBox = theme->load("editbox");
        mazeWidthEditBox->setPosition(75, 90);
        mazeWidthEditBox->setSize(150, 24);
        mazeWidthEditBox->setNumbersOnly();
        mazeWidthEditBox->setText(tgui::to_string(mazeWidth));
        gui.add(mazeWidthEditBox);

        tgui::EditBox::Ptr mazeHeightEditBox = theme->load("editbox");
        mazeHeightEditBox->setPosition(275, 90);
        mazeHeightEditBox->setSize(150, 24);
        mazeHeightEditBox->setNumbersOnly();
        mazeHeightEditBox->setText(tgui::to_string(mazeHeight));
        gui.add(mazeHeightEditBox);

        tgui::EditBox::Ptr tileSizeEditBox = theme->load("editbox");
        tileSizeEditBox->setPosition(475, 90);
        tileSizeEditBox->setSize(150, 24);
        tileSizeEditBox->setNumbersOnly();
        tileSizeEditBox->setText(tgui::to_string(tileSize));
        gui.add(tileSizeEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(20);
        label->setPosition(50, 150);
        label->setText("Speed:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 180);
        label->setText("Player:");
        gui.add(label);

        tgui::EditBox::Ptr playerSpeedEditBox = theme->load("editbox");
        playerSpeedEditBox->setPosition(185, 174);
        playerSpeedEditBox->setSize(200, 24);
        playerSpeedEditBox->setNumbersOnly();
        playerSpeedEditBox->setText(tgui::to_string(playerSpeed));
        gui.add(playerSpeedEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 215);
        label->setText("Enemy walk:");
        gui.add(label);

        tgui::EditBox::Ptr minEnemyWalkSpeedEditBox = theme->load("editbox");
        minEnemyWalkSpeedEditBox->setPosition(185, 209);
        minEnemyWalkSpeedEditBox->setSize(200, 24);
        minEnemyWalkSpeedEditBox->setNumbersOnly();
        minEnemyWalkSpeedEditBox->setText(tgui::to_string(minEnemyWalkSpeed));
        gui.add(minEnemyWalkSpeedEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(400, 215);
        label->setText("up to");
        gui.add(label);

        tgui::EditBox::Ptr maxEnemyWalkSpeedEditBox = theme->load("editbox");
        maxEnemyWalkSpeedEditBox->setPosition(455, 209);
        maxEnemyWalkSpeedEditBox->setSize(200, 24);
        maxEnemyWalkSpeedEditBox->setNumbersOnly();
        maxEnemyWalkSpeedEditBox->setText(tgui::to_string(maxEnemyWalkSpeed));
        gui.add(maxEnemyWalkSpeedEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 250);
        label->setText("Enemy run:");
        gui.add(label);

        tgui::EditBox::Ptr minEnemyRunSpeedEditBox = theme->load("editbox");
        minEnemyRunSpeedEditBox->setPosition(185, 244);
        minEnemyRunSpeedEditBox->setSize(200, 24);
        minEnemyRunSpeedEditBox->setNumbersOnly();
        minEnemyRunSpeedEditBox->setText(tgui::to_string(minEnemyRunSpeed));
        gui.add(minEnemyRunSpeedEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(400, 250);
        label->setText("up to");
        gui.add(label);

        tgui::EditBox::Ptr maxEnemyRunSpeedEditBox = theme->load("editbox");
        maxEnemyRunSpeedEditBox->setPosition(455, 244);
        maxEnemyRunSpeedEditBox->setSize(200, 24);
        maxEnemyRunSpeedEditBox->setNumbersOnly();
        maxEnemyRunSpeedEditBox->setText(tgui::to_string(maxEnemyRunSpeed));
        gui.add(maxEnemyRunSpeedEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(20);
        label->setPosition(50, 300);
        label->setText("Damage:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 330);
        label->setText("Enemy:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(275, 330);
        label->setText("Darkness:");
        gui.add(label);

        tgui::EditBox::Ptr enemyHitDamageEditBox = theme->load("editbox");
        enemyHitDamageEditBox->setPosition(75, 350);
        enemyHitDamageEditBox->setSize(150, 24);
        enemyHitDamageEditBox->setNumbersOnly();
        enemyHitDamageEditBox->setText(tgui::to_string(enemyHitDamage));
        gui.add(enemyHitDamageEditBox);

        tgui::EditBox::Ptr darknessLifeDrainEditBox = theme->load("editbox");
        darknessLifeDrainEditBox->setPosition(275, 350);
        darknessLifeDrainEditBox->setSize(150, 24);
        darknessLifeDrainEditBox->setNumbersOnly();
        darknessLifeDrainEditBox->setText(tgui::to_string(darknessLifeDrain));
        gui.add(darknessLifeDrainEditBox);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(20);
        label->setPosition(50, 405);
        label->setText("Quantities:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(75, 435);
        label->setText("Orbs:");
        gui.add(label);

        label = std::make_shared<tgui::Label>();
        label->setTextSize(16);
        label->setPosition(275, 435);
        label->setText("Enemies:");
        gui.add(label);

        tgui::EditBox::Ptr orbsCountEditBox = theme->load("editbox");
        orbsCountEditBox->setPosition(75, 455);
        orbsCountEditBox->setSize(150, 24);
        orbsCountEditBox->setNumbersOnly();
        orbsCountEditBox->setText(tgui::to_string(orbsCount));
        gui.add(orbsCountEditBox);

        tgui::EditBox::Ptr enemyCountEditBox = theme->load("editbox");
        enemyCountEditBox->setPosition(275, 455);
        enemyCountEditBox->setSize(150, 24);
        enemyCountEditBox->setNumbersOnly();
        enemyCountEditBox->setText(tgui::to_string(enemyCount));
        gui.add(enemyCountEditBox);

        tgui::Button::Ptr resetButton = theme->load("button");
        resetButton->setSize(150, 36);
        resetButton->setPosition(75, 523);
        resetButton->setText("Reset");
        gui.add(resetButton);

        tgui::Button::Ptr startButton = theme->load("button");
        startButton->setSize(200, 50);
        startButton->setPosition(300, 515);
        startButton->setText("Start");
        gui.add(startButton);

        tgui::Button::Ptr helpButton = theme->load("button");
        helpButton->setSize(150, 36);
        helpButton->setPosition(575, 523);
        helpButton->setText("Help");
        gui.add(helpButton);

        bool start = false;
        bool reset = false;
        bool help = false;
        resetButton->connect("pressed", [&](){ reset = true; });
        startButton->connect("pressed", [&](){ start = true; });
        helpButton->connect("pressed", [&](){ help = true; });

        while (window.isOpen())
        {
            sf::Event event;
            while (window.pollEvent(event))
            {
                if (event.type == sf::Event::Closed)
                    window.close();

                gui.handleEvent(event);
            }

            window.clear({200, 200, 200});
            gui.draw();
            window.display();

            if (reset)
            {
                resetSettings();
                break;
            }

            if (help)
            {
                mazeWidth = tgui::stoi(mazeWidthEditBox->getText().toAnsiString());
                mazeHeight = tgui::stoi(mazeHeightEditBox->getText().toAnsiString());
                tileSize = tgui::stof(tileSizeEditBox->getText().toAnsiString());
                playerSpeed = tgui::stof(playerSpeedEditBox->getText().toAnsiString());
                minEnemyWalkSpeed = tgui::stof(minEnemyWalkSpeedEditBox->getText().toAnsiString());
                minEnemyRunSpeed = tgui::stof(minEnemyRunSpeedEditBox->getText().toAnsiString());
                maxEnemyWalkSpeed = tgui::stof(maxEnemyWalkSpeedEditBox->getText().toAnsiString());
                maxEnemyRunSpeed = tgui::stof(maxEnemyRunSpeedEditBox->getText().toAnsiString());
                orbsCount = tgui::stoi(orbsCountEditBox->getText().toAnsiString());
                enemyCount = tgui::stoi(enemyCountEditBox->getText().toAnsiString());
                enemyHitDamage = tgui::stof(enemyHitDamageEditBox->getText().toAnsiString());
                darknessLifeDrain = tgui::stof(darknessLifeDrainEditBox->getText().toAnsiString());

                showHelpScreen();
                break;
            }

            if (start)
            {
                mazeWidth = tgui::stoi(mazeWidthEditBox->getText().toAnsiString());
                mazeHeight = tgui::stoi(mazeHeightEditBox->getText().toAnsiString());
                tileSize = tgui::stof(tileSizeEditBox->getText().toAnsiString());
                playerSpeed = tgui::stof(playerSpeedEditBox->getText().toAnsiString());
                minEnemyWalkSpeed = tgui::stof(minEnemyWalkSpeedEditBox->getText().toAnsiString());
                minEnemyRunSpeed = tgui::stof(minEnemyRunSpeedEditBox->getText().toAnsiString());
                maxEnemyWalkSpeed = tgui::stof(maxEnemyWalkSpeedEditBox->getText().toAnsiString());
                maxEnemyRunSpeed = tgui::stof(maxEnemyRunSpeedEditBox->getText().toAnsiString());
                orbsCount = tgui::stoi(orbsCountEditBox->getText().toAnsiString());
                enemyCount = tgui::stoi(enemyCountEditBox->getText().toAnsiString());
                enemyHitDamage = tgui::stof(enemyHitDamageEditBox->getText().toAnsiString());
                darknessLifeDrain = tgui::stof(darknessLifeDrainEditBox->getText().toAnsiString());

                setup();
                play();
                break;
            }
        }
    }
}

void Game::showHelpScreen()
{
    bool toMenu = false;
    gui.removeAllWidgets();

    tgui::Label::Ptr label = std::make_shared<tgui::Label>();
    gui.add(label);
    label->setTextSize(36);
    label->setText("Help");
    label->setPosition((windowWidth - label->getSize().x) / 2.f, 40);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(22);
    label->setText("Energy orb");
    label->setPosition(200, 130);
    gui.add(label);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(18);
    label->setText("Needed to keep your light shining, you can't survive\nin the darkness. Collect all of these to win.");
    label->setPosition(200, 170);
    gui.add(label);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(22);
    label->setText("Direction indicator");
    label->setPosition(200, 240);
    gui.add(label);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(18);
    label->setText("Points in the direction of the nearest energy orb.");
    label->setPosition(200, 280);
    gui.add(label);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(22);
    label->setText("Human");
    label->setPosition(200, 340);
    gui.add(label);

    label = std::make_shared<tgui::Label>();
    label->setTextSize(18);
    label->setText("They think you are a dangerous monster and they will\ntry to kill you. You must avoid them.");
    label->setPosition(200, 380);
    gui.add(label);

    tgui::Button::Ptr button = std::make_shared<tgui::Theme>("TGUI/widgets/Black.txt")->load("button");
    button->setText("Back To Menu");
    button->setSize(300, 50);
    button->setPosition((windowWidth - button->getSize().x) / 2.f, 515);
    button->connect("pressed", [&](){ toMenu = true; });
    gui.add(button);

    enemyLightSprite.setScale({80 / 100.f, 80 / 100.f});
    orbBackSprite.setScale({80 / 100.f, 80 / 100.f});
    orbFrontSprite.setScale({80 / 100.f, 80 / 100.f});

    currentEnemyTexture = 0;
    enemySprite.setTexture(enemyTextures[Direction::Down].front());
    enemySprite.setScale({80 / 100.f, 80 / 100.f});

    bool orbTimerIncreasing = true;
    float orbTimer = 0;
    float enemyTimer = 0;
    sf::Clock clock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        float elapsedTime = clock.restart().asSeconds();

        enemyTimer += elapsedTime;
        if (enemyTimer >= 0.35)
        {
            if (++currentEnemyTexture == enemyTextures.begin()->second.size())
                currentEnemyTexture = 0;

            enemySprite.setTexture(enemyTextures.begin()->second[currentEnemyTexture]);
            enemyTimer = 0;
        }

        if (orbTimerIncreasing)
        {
            orbTimer += elapsedTime * 600;
            if (orbTimer >= 255)
            {
                orbTimer = 255;
                orbTimerIncreasing = false;
            }
        }
        else
        {
            orbTimer -= elapsedTime * 600;
            if (orbTimer <= 0)
            {
                orbTimer = 0;
                orbTimerIncreasing = true;
            }
        }

        if (toMenu)
            break;

        window.clear({200, 200, 200});

        // Draw the orb
        orbBackSprite.setPosition(100, 100);
        window.draw(orbBackSprite);
        orbFrontSprite.setPosition(100, 100);
        orbFrontSprite.setColor({255, 255, 255, (sf::Uint8)(255-orbTimer)});
        window.draw(orbFrontSprite);

        // Draw the direction arrow
        directionArrowSprite.rotate(elapsedTime * 120);
        directionArrowSprite.setPosition(140, 250);
        window.draw(directionArrowSprite);

        // Draw the enemy
        enemySprite.setTexture(enemyTextures[Direction::Down][currentEnemyTexture]);
        enemySprite.setPosition(110, 330);
        window.draw(enemySprite);

        gui.draw();
        window.display();
    }
}

void Game::showGameOverScreen(bool victory)
{
    bool toMenu = false;

    gui.removeAllWidgets();

    tgui::Panel::Ptr panel = std::make_shared<tgui::Panel>();
    panel->setBackgroundColor({0, 0, 0, 100});
    panel->setSize(windowWidth, windowHeight);
    gui.add(panel);

    tgui::Label::Ptr label = std::make_shared<tgui::Label>();
    panel->add(label);
    if (victory)
        label->setText("You won");
    else
        label->setText("Game Over");
    label->setTextSize(60);
    label->setTextColor(sf::Color::White);
    label->setPosition((windowWidth - label->getSize().x) / 2.f, (windowHeight / 3.f) - (label->getSize().y / 2.f));

    tgui::Button::Ptr button = std::make_shared<tgui::Theme>("TGUI/widgets/Black.txt")->load("button");
    button->setText("Back To Menu");
    button->setSize(300, 50);
    button->setPosition((windowWidth - button->getSize().x) / 2.f, (windowHeight * 2.f / 3.f) - (button->getSize().y / 2.f));
    button->connect("pressed", [&](){ toMenu = true; });
    panel->add(button);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            gui.handleEvent(event);
        }

        window.clear({200, 200, 200});
        gui.draw();
        window.display();

        if (toMenu)
            break;
    }
}

void Game::resetSettings()
{
    mazeWidth = 48;
    mazeHeight = 48;
    tileSize = 70;
    playerSpeed = 5;
    minEnemyWalkSpeed = 1;
    minEnemyRunSpeed = 2.75;
    maxEnemyWalkSpeed = 2.5;
    maxEnemyRunSpeed = 4.5;
    orbsCount = 20;
    enemyCount = 10;
    enemyHitDamage = 40; // 40% of life
    darknessLifeDrain = 3; // 3% of life per second
}

void Game::setup()
{
    sounds.clear();

    directionArrowSprite.setPosition(20, 20);

    enemyLightSprite.setScale({tileSize / 100.f, tileSize / 100.f});
    orbBackSprite.setScale({tileSize / 100.f, tileSize / 100.f});
    orbFrontSprite.setScale({tileSize / 100.f, tileSize / 100.f});

    currentPlayerTexture = 0;
    playerSprite.setTexture(playerTextures[Direction::Down].front());
    playerSprite.setScale({tileSize / 100.f, tileSize / 100.f});
    playerSprite.setOrigin({playerTextures[Direction::Down].front().getSize().x / 2.f, playerTextures[Direction::Down].front().getSize().y / 2.f});

    currentEnemyTexture = 0;
    enemySprite.setTexture(enemyTextures[Direction::Down].front());
    enemySprite.setScale({tileSize / 100.f, tileSize / 100.f});
    enemySprite.setOrigin({enemyTextures[Direction::Down].front().getSize().x / 2.f, enemyTextures[Direction::Down].front().getSize().y / 2.f});

    MazeGenerator mazeGenerator{{mazeWidth, mazeHeight}};
    maze = mazeGenerator.getMaze();

    player.square = mazeGenerator.getStartPos();
    player.moving = false;
    player.direction = Direction::Down;
    player.life = 100;

    // Count the space in the maze
    unsigned walkableCount = 0;
    for (unsigned i = 0; i < maze.size(); ++i) {
        for (unsigned j = 0; j < maze[i].size(); ++j) {
            if (maze[i][j] == ' ')
                walkableCount += 1;
        }
    }

    // Place random orbs
    std::set<int> orbs;
    std::uniform_int_distribution<int> orbsDistribution(0, walkableCount-2);
    for (unsigned int i = 0; i < std::min(orbsCount, walkableCount-1); ++i)
    {
        int pos = orbsDistribution(generator);
        while (orbs.find(pos) != orbs.end())
            pos = orbsDistribution(generator);

        orbs.insert(pos);
    }

    unsigned num = 0;
    orbList.clear();
    for (unsigned i = 0; i < maze.size(); ++i) {
        for (unsigned j = 0; j < maze[i].size(); ++j) {
            if ((maze[i][j] == ' ') && (j != player.square.x || i != player.square.y))
            {
                num++;
                if (orbs.find(num) != orbs.end())
                    orbList.push_back({j, i});
            }
        }
    }

    // Place random enemies
    std::set<int> enemiesSet;
    std::uniform_int_distribution<int> enemyDistribution(0, walkableCount-5);
    for (unsigned int i = 0; i < std::min(enemyCount, walkableCount-4); ++i)
    {
        int pos = enemyDistribution(generator);
        while (enemiesSet.find(pos) != enemiesSet.end())
            pos = enemyDistribution(generator);

        enemiesSet.insert(pos);
    }

    num = 0;
    enemies.clear();
    for (unsigned i = 0; i < maze.size(); ++i) {
        for (unsigned j = 0; j < maze[i].size(); ++j) {
            if ((maze[i][j] == ' ') && (j != player.square.x || i != player.square.y) && (j != player.square.x-1 || i != player.square.y) && (j != player.square.x+1 || i != player.square.y) && (j != player.square.x || i != player.square.y-1) && (j != player.square.x || i != player.square.y+1))
            {
                num++;
                if (enemiesSet.find(num) != enemiesSet.end())
                {
                    enemies.push_back({});
                    enemies.back().square = {j,i};
                    enemies.back().speed = minEnemyWalkSpeed;
                    enemies.back().pos = {enemies.back().square.x * tileSize + (tileSize / 2.f), enemies.back().square.y * tileSize + (tileSize / 2.f)};
                }
            }
        }
    }

    gui.removeAllWidgets();

    healthBar = std::make_shared<tgui::ProgressBar>();
    healthBar->setPosition(40, 10);
    healthBar->setSize(140, 20);
    healthBar->setMaximum(100);
    healthBar->setValue(100);
    healthBar->getRenderer()->setTextColor(sf::Color::Black);
    healthBar->getRenderer()->setBorderColor(sf::Color::Black);
    healthBar->getRenderer()->setBackgroundColor(sf::Color::White);
    healthBar->getRenderer()->setForegroundColor(sf::Color::Green);
    gui.add(healthBar);
}

void Game::play()
{
    view = window.getView();
    player.pos = {player.square.x * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};

    enemyWalkSpeed = minEnemyWalkSpeed;
    enemyRunSpeed = minEnemyRunSpeed;

    lightTimer = 0;
    playerTimer = 0;
    orbTimer = 0;
    orbTimerIncreasing = true;

    sf::Clock clock;
    bool pause = false;
    bool toMenu = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    pause = !pause;
                    if (pause)
                    {
                        tgui::Panel::Ptr panel = std::make_shared<tgui::Panel>();
                        panel->setBackgroundColor({0, 0, 0, 100});
                        panel->setSize(windowWidth, windowHeight);
                        gui.add(panel, "PausePanel");

                        tgui::Label::Ptr label = std::make_shared<tgui::Label>();
                        panel->add(label);
                        label->setText("Paused");
                        label->setTextSize(60);
                        label->setTextColor(sf::Color::White);
                        label->setPosition((windowWidth - label->getSize().x) / 2.f, (windowHeight / 3.f) - (label->getSize().y / 2.f));

                        tgui::Button::Ptr button = std::make_shared<tgui::Theme>("TGUI/widgets/Black.txt")->load("button");
                        button->setText("Resume");
                        button->setSize(300, 50);
                        button->setPosition((windowWidth - button->getSize().x) / 2.f, (windowHeight * 1.7f / 3.f) - (button->getSize().y / 2.f));
                        button->connect("pressed", [&](){ gui.remove(gui.get("PausePanel")); pause = false; });
                        panel->add(button);

                        button = std::make_shared<tgui::Theme>("TGUI/widgets/Black.txt")->load("button");
                        button->setText("Exit To Menu");
                        button->setSize(300, 50);
                        button->setPosition((windowWidth - button->getSize().x) / 2.f, (windowHeight * 2.2f / 3.f) - (button->getSize().y / 2.f));
                        button->connect("pressed", [&](){ toMenu = true; });
                        panel->add(button);
                    }
                    else
                        gui.remove(gui.get("PausePanel"));
                }
            }

            gui.handleEvent(event);
        }

        // Remove sounds that have finished
        std::vector<std::list<sf::Sound>::iterator> soundsToRemove;
        for (auto it = sounds.begin(); it != sounds.end(); ++it)
        {
            if (it->getStatus() == sf::SoundSource::Status::Stopped)
                soundsToRemove.push_back(it);
        }
        for (auto it : soundsToRemove)
            sounds.erase(it);

        float elapsedTime = clock.restart().asSeconds();
        if (!pause)
            updateGame(elapsedTime);

        if (orbList.empty())
        {
            showGameOverScreen(true);
            return;
        }
        if (player.life == 0)
        {
            showGameOverScreen(false);
            return;
        }

        if (toMenu)
            return;

        drawGame();
    }
}

void Game::updateGame(float elapsedTime)
{
    lightTimer += elapsedTime;
    playerTimer += elapsedTime;

    if (lightTimer >= 20)
    {
        player.life -= elapsedTime * darknessLifeDrain;
        lightTimer = 20;
    }

    if (playerTimer >= 0.2)
    {
        playerTimer = 0;
        if (player.moving)
        {
            if (++currentPlayerTexture >= playerTextures[player.direction].size())
                currentPlayerTexture = 0;

            playerSprite.setTexture(playerTextures[player.direction][currentPlayerTexture]);
        }

        if (++currentEnemyTexture == enemyTextures.begin()->second.size())
            currentEnemyTexture = 0;

        enemySprite.setTexture(enemyTextures.begin()->second[currentEnemyTexture]);
    }

    if (orbTimerIncreasing)
    {
        orbTimer += elapsedTime * 600;
        if (orbTimer >= 255)
        {
            orbTimer = 255;
            orbTimerIncreasing = false;
        }
    }
    else
    {
        orbTimer -= elapsedTime * 600;
        if (orbTimer <= 0)
        {
            orbTimer = 0;
            orbTimerIncreasing = true;
        }
    }

    // Move the player
    if (player.moving)
    {
        if (player.pos.x == player.moveTo.x)
        {
            if (player.pos.y < player.moveTo.y)
            {
                player.pos.y += elapsedTime * playerSpeed * tileSize;
                if (player.pos.y >= player.moveTo.y)
                {
                    player.pos.y = player.moveTo.y;
                    player.moving = false;
                    player.square.y++;
                }
            }
            else // Moving up
            {
                player.pos.y -= elapsedTime * playerSpeed * tileSize;
                if (player.pos.y <= player.moveTo.y)
                {
                    player.pos.y = player.moveTo.y;
                    player.moving = false;
                    player.square.y--;
                }
            }
        }
        else // Moving horizontal
        {
            if (player.pos.x < player.moveTo.x)
            {
                player.pos.x += elapsedTime * playerSpeed * tileSize;
                if (player.pos.x >= player.moveTo.x)
                {
                    player.pos.x = player.moveTo.x;
                    player.moving = false;
                    player.square.x++;
                }
            }
            else // Moving left
            {
                player.pos.x -= elapsedTime * playerSpeed * tileSize;
                if (player.pos.x <= player.moveTo.x)
                {
                    player.pos.x = player.moveTo.x;
                    player.moving = false;
                    player.square.x--;
                }
            }
        }

        // We just finished our move
        if (!player.moving)
        {
            // Pick up food when walking over it
            auto it = std::find(orbList.begin(), orbList.end(), sf::Vector2u(player.square.x, player.square.y));
            if (it != orbList.end())
            {
                orbList.erase(it);
                if (orbList.empty())
                {
                    sounds.push_back({});
                    sounds.back().setBuffer(soundBufferVictory);
                    sounds.back().play();
                    return;
                }

                sounds.push_back({});
                sounds.back().setBuffer(soundBufferPickup);
                sounds.back().play();

                lightTimer -= 10;
                if (lightTimer < -4)
                    lightTimer = -4;

                // Enemies are now faster
                float oldEnemyRunSpeed = enemyRunSpeed;
                enemyWalkSpeed = minEnemyWalkSpeed + ((1-(orbList.size() / (float)orbsCount)) * (maxEnemyWalkSpeed - minEnemyWalkSpeed));
                enemyRunSpeed = minEnemyRunSpeed + ((1-(orbList.size() / (float)orbsCount)) * (maxEnemyRunSpeed - minEnemyRunSpeed));
                for (auto& enemy : enemies)
                {
                    if (enemy.speed == oldEnemyRunSpeed)
                        enemy.speed = enemyRunSpeed;
                    else
                        enemy.speed = enemyWalkSpeed;
                }
            }

            // Kills the player when walking over an enemy
            for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt)
            {
                if (enemyIt->square.x == player.square.x && enemyIt->square.y == player.square.y)
                {
                    player.life -= enemyHitDamage;
                    lightTimer += 5;
                    enemies.erase(enemyIt);

                    sounds.push_back({});
                    sounds.back().setBuffer(soundBufferHit);
                    sounds.back().play();
                    break;
                }
            }
        }
    }
    else // Standing still
    {
        if ((maze[player.square.y-1][player.square.x] != '#')
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::W))))
        {
            player.moving = true;
            player.direction = Direction::Up;
            player.moveFrom = {player.square.x * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.moveTo = {player.square.x * tileSize + (tileSize / 2.f), (player.square.y-1) * tileSize + (tileSize / 2.f)};
            player.pos = player.moveFrom;
        }
        else if ((maze[player.square.y][player.square.x+1] != '#')
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::D))))
        {
            player.moving = true;
            player.direction = Direction::Right;
            player.moveFrom = {player.square.x * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.moveTo = {(player.square.x+1) * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.pos = player.moveFrom;
        }
        else if ((maze[player.square.y+1][player.square.x] != '#')
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::S))))
        {
            player.moving = true;
            player.direction = Direction::Down;
            player.moveFrom = {player.square.x * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.moveTo = {player.square.x * tileSize + (tileSize / 2.f), (player.square.y+1) * tileSize + (tileSize / 2.f)};
            player.pos = player.moveFrom;
        }
        else if ((maze[player.square.y][player.square.x-1] != '#')
         && ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
          || (sf::Keyboard::isKeyPressed(sf::Keyboard::A))))
        {
            player.moving = true;
            player.direction = Direction::Left;
            player.moveFrom = {player.square.x * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.moveTo = {(player.square.x-1) * tileSize + (tileSize / 2.f), player.square.y * tileSize + (tileSize / 2.f)};
            player.pos = player.moveFrom;
        }
    }

    // Move the enemies
    std::vector<std::list<Enemy>::iterator> enemiesToRemove;
    for (auto enemyIt = enemies.begin(); enemyIt != enemies.end(); ++enemyIt)
    {
        if (enemyIt->moving)
        {
            if (enemyIt->pos.x == enemyIt->moveTo.x)
            {
                if (enemyIt->pos.y < enemyIt->moveTo.y)
                {
                    enemyIt->pos.y += elapsedTime * enemyIt->speed * tileSize;
                    if (enemyIt->pos.y >= enemyIt->moveTo.y)
                    {
                        enemyIt->pos.y = enemyIt->moveTo.y;
                        enemyIt->moving = false;
                        enemyIt->square.y++;
                    }
                }
                else // Moving up
                {
                    enemyIt->pos.y -= elapsedTime * enemyIt->speed * tileSize;
                    if (enemyIt->pos.y <= enemyIt->moveTo.y)
                    {
                        enemyIt->pos.y = enemyIt->moveTo.y;
                        enemyIt->moving = false;
                        enemyIt->square.y--;
                    }
                }
            }
            else // Moving horizontal
            {
                if (enemyIt->pos.x < enemyIt->moveTo.x)
                {
                    enemyIt->pos.x += elapsedTime * enemyIt->speed * tileSize;
                    if (enemyIt->pos.x >= enemyIt->moveTo.x)
                    {
                        enemyIt->pos.x = enemyIt->moveTo.x;
                        enemyIt->moving = false;
                        enemyIt->square.x++;
                    }
                }
                else // Moving left
                {
                    enemyIt->pos.x -= elapsedTime * enemyIt->speed * tileSize;
                    if (enemyIt->pos.x <= enemyIt->moveTo.x)
                    {
                        enemyIt->pos.x = enemyIt->moveTo.x;
                        enemyIt->moving = false;
                        enemyIt->square.x--;
                    }
                }
            }

            // The enemy just finished his move
            if (!enemyIt->moving)
            {
                // Kills the player when walking over him
                if (enemyIt->square.x == player.square.x && enemyIt->square.y == player.square.y)
                {
                    player.life -= enemyHitDamage;
                    lightTimer += 5;
                    enemiesToRemove.push_back(enemyIt);

                    sounds.push_back({});
                    sounds.back().setBuffer(soundBufferHit);
                    sounds.back().play();
                }
            }
        }
        else // Enemy is currently standing still
        {
            // Walk in a direction without going back unless necessary
            std::vector<sf::Vector2i> possibleDirections;
            if (maze[enemyIt->square.y][enemyIt->square.x-1] == ' ')
                possibleDirections.emplace_back(-1, 0);
            if (maze[enemyIt->square.y][enemyIt->square.x+1] == ' ')
                possibleDirections.emplace_back(1, 0);
            if (maze[enemyIt->square.y-1][enemyIt->square.x] == ' ')
                possibleDirections.emplace_back(0, -1);
            if (maze[enemyIt->square.y+1][enemyIt->square.x] == ' ')
                possibleDirections.emplace_back(0, 1);

            if (possibleDirections.size() == 1)
                enemyIt->direction = possibleDirections[0];
            else
            {
                auto eraseIt = std::find(possibleDirections.begin(), possibleDirections.end(), -enemyIt->direction);
                if ((eraseIt != possibleDirections.end()) && (possibleDirections.size() == 1))
                {
                    possibleDirections.erase(eraseIt);
                    enemyIt->direction = possibleDirections[0];

                    // If the enemy sees the player then it should run towards it
                    if (((player.square.x == enemyIt->square.x + enemyIt->direction.x) && (player.square.y == enemyIt->square.y + enemyIt->direction.y))
                     || ((player.square.x == enemyIt->square.x + 2*enemyIt->direction.x) && (player.square.y == enemyIt->square.y + 2*enemyIt->direction.y))
                     || ((player.square.x == enemyIt->square.x + 3*enemyIt->direction.x) && (player.square.y == enemyIt->square.y + 3*enemyIt->direction.y) && (maze[enemyIt->square.y + 2*enemyIt->direction.y][enemyIt->square.x + 2*enemyIt->direction.x] == ' ')))
                    {
                        enemyIt->speed = enemyRunSpeed;
                    }
                }
                else
                {
                    if (eraseIt != possibleDirections.end())
                        possibleDirections.erase(eraseIt);

                    // If the enemy sees the player then it should run towards it
                    bool directionChosen = false;
                    for (auto& dir : possibleDirections)
                    {
                        if (((player.square.x == enemyIt->square.x + dir.x) && (player.square.y == enemyIt->square.y + dir.y))
                         || ((player.square.x == enemyIt->square.x + 2*dir.x) && (player.square.y == enemyIt->square.y + 2*dir.y))
                         || ((player.square.x == enemyIt->square.x + 3*dir.x) && (player.square.y == enemyIt->square.y + 3*dir.y) && (maze[enemyIt->square.y + 2*dir.y][enemyIt->square.x + 2*dir.x] == ' ')))
                        {
                            enemyIt->direction = dir;
                            enemyIt->speed = enemyRunSpeed;
                            directionChosen = true;
                        }
                    }

                    // Let the enemy move in the same direction when it was tracking the player
                    if (!directionChosen && enemyIt->speed == enemyRunSpeed)
                    {
                        if (std::find(possibleDirections.begin(), possibleDirections.end(), enemyIt->direction) != possibleDirections.end())
                            directionChosen = true;
                    }

                    // Pick a random direction
                    if (!directionChosen)
                    {
                        std::uniform_int_distribution<int> enemyDistribution(0, possibleDirections.size()-1);

                        enemyIt->speed = enemyWalkSpeed;
                        enemyIt->direction = possibleDirections[enemyDistribution(generator)];
                    }
                }
            }

            enemyIt->moveFrom = {enemyIt->square.x * tileSize + (tileSize / 2.f), enemyIt->square.y * tileSize + (tileSize / 2.f)};
            enemyIt->moveTo = {(enemyIt->square.x + enemyIt->direction.x) * tileSize + (tileSize / 2.f), (enemyIt->square.y + enemyIt->direction.y) * tileSize + (tileSize / 2.f)};
            enemyIt->pos = enemyIt->moveFrom;
            enemyIt->moving = true;
        }
    }

    for (auto& it : enemiesToRemove)
        enemies.erase(it);

    // Rotate the arrow towards the closest orb
    int closestOrbDistance = std::numeric_limits<int>::max();
    sf::Vector2u closestOrb;
    for (auto& orbPos : orbList)
    {
        if (closestOrbDistance > std::abs((int)orbPos.y - (int)player.square.y) + std::abs((int)orbPos.x - (int)player.square.x))
        {
            closestOrbDistance = std::abs((int)orbPos.y - (int)player.square.y) + std::abs((int)orbPos.x - (int)player.square.x);
            closestOrb = orbPos;
        }
    }

    if (closestOrb.x == player.square.x)
    {
        if (closestOrb.y < player.square.y)
            directionArrowSprite.setRotation(0);
        else
            directionArrowSprite.setRotation(180);
    }
    else
    {
        directionArrowSprite.setRotation(90 + std::atan2((int)closestOrb.y - (int)player.square.y, (int)closestOrb.x - (int)player.square.x) / 3.141529f * 180.f);
    }

    // Update the life and the health bar
    if (player.life < 0)
        player.life = 0;

    healthBar->setValue((int)player.life);
    healthBar->setText(tgui::to_string((int)player.life) + "/100");
    if (player.life >= 50)
    {
        healthBar->getRenderer()->setForegroundColor({(sf::Uint8)(255.f - ((player.life-50) / 50.f * 255.f)), 255, 0});
        healthBar->getRenderer()->setTextColorFront(sf::Color::Blue);
    }
    else
    {
        healthBar->getRenderer()->setForegroundColor({255, (sf::Uint8)(player.life / 50.f * 255.f), 0});
        healthBar->getRenderer()->setTextColorFront(sf::Color::Blue);
    }

    if (player.life == 0)
    {
        sounds.push_back({});
        sounds.back().setBuffer(soundBufferGameOver);
        sounds.back().play();
    }
}

void Game::drawGame()
{
    view.setCenter({std::round(player.pos.x), std::round(player.pos.y)});
    window.setView(view);

    window.clear();

    // Draw the maze
    for (unsigned i = 0; i < maze.size(); ++i)
    {
        for (unsigned j = 0; j < maze[i].size(); ++j)
        {
            sf::RectangleShape rect{{tileSize, tileSize}};
            rect.setPosition(j*tileSize, i*tileSize);

            if (maze[i][j] == ' ')
            {
                if ((i+j)%2)
                    rect.setFillColor({190, 190, 190});
                else
                    rect.setFillColor({210, 210, 210});
            }
            else
                rect.setFillColor({80, 80, 80});

            window.draw(rect);
        }
    }

    // Draw the orbs
    for (auto& orbPosition : orbList)
    {
        orbBackSprite.setPosition(orbPosition.x*tileSize, orbPosition.y*tileSize);
        window.draw(orbBackSprite);

        orbFrontSprite.setPosition(orbPosition.x*tileSize, orbPosition.y*tileSize);
        orbFrontSprite.setColor({255, 255, 255, (sf::Uint8)(255-orbTimer)});
        window.draw(orbFrontSprite);
    }

    // Draw the player
    playerSprite.setTexture(playerTextures[player.direction][currentPlayerTexture]);
    playerSprite.setPosition(view.getCenter());
    window.draw(playerSprite);

    // Draw the enemies
    for (auto& enemy : enemies)
    {
        if (enemy.direction.x == 1)
        {
            enemySprite.setTexture(enemyTextures[Direction::Right][currentEnemyTexture]);
            enemySprite.setPosition(enemy.pos);
            window.draw(enemySprite);
        }
        else if (enemy.direction.x == -1)
        {
            enemySprite.setTexture(enemyTextures[Direction::Left][currentEnemyTexture]);
            enemySprite.setPosition(enemy.pos);
            window.draw(enemySprite);
        }
        else if (enemy.direction.y == -1)
        {
            enemySprite.setTexture(enemyTextures[Direction::Up][currentEnemyTexture]);
            enemySprite.setPosition(enemy.pos);
            window.draw(enemySprite);
        }
        else
        {
            enemySprite.setTexture(enemyTextures[Direction::Down][currentEnemyTexture]);
            enemySprite.setPosition(enemy.pos);
            window.draw(enemySprite);
        }
    }

    // Draw the light
    window.setView(window.getDefaultView());
    window.draw(playerLightSprite, sf::BlendMultiply);

    sf::RectangleShape darknessRectShape{window.getView().getSize()};
    if (lightTimer < 0)
        darknessRectShape.setFillColor({255, 255, 255, (sf::Uint8)(-lightTimer * 5.f)});
    else if (lightTimer < 20)
        darknessRectShape.setFillColor({0, 0, 0, (sf::Uint8)((lightTimer / 20.f) * 200.f)});
    else
        darknessRectShape.setFillColor({0, 0, 0, 200});
    window.draw(darknessRectShape);

    window.draw(directionArrowSprite);

    gui.draw();
    window.display();
}
