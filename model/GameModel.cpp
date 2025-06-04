#include "GameModel.h"
#include <mutex>

GameModel::GameModel()
    : m_cameraX{ 0 }, m_worldWidth{ 2000 }, m_isGameOver{ false }, m_isRunning{ false },
    m_rng{ std::random_device()() }, m_dist{ 0.0, 1.0 } {
    reset();
}

GameModel::~GameModel() {
    stop();
    clearAllObjects();
}

double GameModel::getCameraX() const {
    return m_cameraX;
}

Player* GameModel::getPlayer() {
    return m_player.get();
}

bool GameModel::getGameOver() const { 
    return m_isGameOver;
}

bool GameModel::isGameRunning() const { 
    return m_isRunning;
}

void GameModel::start() {
    if (m_isRunning) {
        stop();
    }

    m_isRunning = true;
    m_isGameOver = false;

    m_updateThread = std::thread([this]() {
        using clock = std::chrono::steady_clock;
        const auto updateInterval = std::chrono::microseconds(1000000 / 60);

        while (!m_isGameOver && m_isRunning) {
            auto start = clock::now();
            update();
            auto end = clock::now();

            auto elapsed = end - start;
            if (elapsed < updateInterval) {
                std::this_thread::sleep_for(updateInterval - elapsed);
            }
        }
        });
}

void GameModel::stop() {
    m_isGameOver = true;
    m_isRunning = false;
    if (m_updateThread.joinable()) {
        m_updateThread.join();
    }
}

void GameModel::reset() {
    bool wasRunning = m_isRunning;
    if (wasRunning) {
        stop();
    }

    {
        std::lock_guard<std::mutex> lock(m_dataMutex);
        m_platforms.clear();
        m_enemies.clear();
        m_coins.clear();

        m_platforms.shrink_to_fit();
        m_enemies.shrink_to_fit();
        m_coins.shrink_to_fit();

        if (m_player) {
            m_player.reset();
        }
        m_player = std::make_unique<Player>(100, 300);
        m_player->reset();

        m_cameraX = 0;
        m_worldWidth = 2000;
        m_isGameOver = false;

        m_platforms.push_back(std::make_unique<Platform>(0, 550, 800, 20));

        m_enemies.push_back(std::make_unique<Enemy>(200, 510, 2, false));
        m_enemies.push_back(std::make_unique<Enemy>(400, 510, 2, true));
        m_enemies.push_back(std::make_unique<Enemy>(600, 510, 3, false));

        generateWorldSegment(800, m_worldWidth);
    }

    if (wasRunning) {
        start();
    }
}

void GameModel::clearAllObjects() {
    std::lock_guard<std::mutex> lock(m_dataMutex);

    m_platforms.clear();
    m_enemies.clear();
    m_coins.clear();

    m_platforms.shrink_to_fit();
    m_enemies.shrink_to_fit();
    m_coins.shrink_to_fit();

    if (m_player) {
        m_player.reset();
    }
}

void GameModel::update() {
    if (!m_isRunning || m_isGameOver) return;

    std::lock_guard<std::mutex> lock(m_dataMutex);

    if (!m_player) {
        return;
    }

    m_player->update();

    for (auto& enemy : m_enemies) {
        if (enemy) {
            enemy->update();
            enemy->applyGravity(m_platforms);
        }
    }

    if (m_player->getX() > m_cameraX + 400) {
        m_cameraX = m_player->getX() - 400;
    }

    if (m_player->getX() > m_worldWidth - 1000) {
        m_worldWidth += 1000;
        generateMoreWorld();
    }

    checkCollisions();

    m_enemies.erase(std::remove_if(m_enemies.begin(), m_enemies.end(),
        [](const auto& e) {
            return !e || !e->getActive();
        }), m_enemies.end());

    m_coins.erase(std::remove_if(m_coins.begin(), m_coins.end(),
        [](const auto& c) {
            return !c || !c->getActive();
        }), m_coins.end());

    if (!m_player->getActive()) {
        m_isGameOver = true;
        m_isRunning = false;
    }
}

void GameModel::checkCollisions() {
    if (!m_player) return;

    for (auto& platform : m_platforms) {
        if (platform && platform->getActive() && m_player->collidesWith(*platform)) {
            if (m_player->getY() + m_player->getHeight() > platform->getY() &&
                m_player->getY() < platform->getY()) {
                m_player->setPosition(m_player->getX(), platform->getY() - m_player->getHeight());
                static_cast<Player*>(m_player.get())->stopJump();
            }
        }
    }

    for (auto& enemy : m_enemies) {
        if (enemy && enemy->getActive() && m_player->collidesWith(*enemy)) {
            Player* playerPtr = static_cast<Player*>(m_player.get());
            double playerBottom = m_player->getY() + m_player->getHeight();
            double playerTop = m_player->getY();
            double playerVelocityY = playerPtr->getVelocityY();

            double enemyTop = enemy->getY();
            double enemyBottom = enemy->getY() + enemy->getHeight();
            bool playerAboveEnemy = (playerBottom <= enemyTop + 15);
            bool playerFalling = (playerVelocityY > 1);

            if (playerAboveEnemy && playerFalling) {
                enemy->setActive(false);
                playerPtr->addCoin();
            }
            else {
                if (!playerPtr->getIsInvulnerable()) {
                    playerPtr->takeDamage();

                    if (playerPtr->getLives() <= 0) {
                        m_isGameOver = true;
                        m_isRunning = false;
                    }
                }
            }
        }
    }

    for (auto& coin : m_coins) {
        if (coin && coin->getActive() && m_player->collidesWith(*coin)) {
            m_player->addCoin();
            coin->setActive(false);
        }
    }
}

void GameModel::generateMoreWorld() {
    generateWorldSegment(m_worldWidth - 500, m_worldWidth + 500);
}

void GameModel::generateWorldSegment(double startX, double endX) {
    double currentX = startX;
    double lastHeight = 550;

    double bottomPlatformY = 599;
    m_platforms.push_back(std::make_unique<Platform>(
        currentX, bottomPlatformY, endX - startX + 500, 50));

    while (currentX < endX) {
        double segmentWidth = 150 + m_dist(m_rng) * 2;
        double segmentHeight = lastHeight - 50 + m_dist(m_rng) * 120;
        segmentHeight = std::max(350.0, std::min(580.0, segmentHeight));

        if (segmentHeight < bottomPlatformY - 50) {
            double platformWidth = 120 + m_dist(m_rng) * 80;
            m_platforms.push_back(std::make_unique<Platform>(
                currentX, segmentHeight, platformWidth, 20));

            if (m_dist(m_rng) > 0.1) {
                for (int i = 0; i < 2; i++) {
                    m_coins.push_back(std::make_unique<Coin>(
                        currentX + 20 + i * 40, segmentHeight - 50));
                }
            }

            if (m_dist(m_rng) > 0.43) {
                double enemyX = currentX + 30;
                double enemyY = segmentHeight - 40;

                try {
                    auto enemy = std::make_unique<Enemy>(enemyX, enemyY, 1.5 + m_dist(m_rng) * 1.5, m_dist(m_rng) > 0.5);
                    if (enemy) {
                        m_enemies.push_back(std::move(enemy));
                    }
                }
                catch (const std::exception& e) {
                }
            }
        }

        currentX += segmentWidth;
        lastHeight = segmentHeight;
    }
}

std::vector<std::unique_ptr<Platform>>& GameModel::getPlatforms() {
    return m_platforms;
}

std::vector<std::unique_ptr<Enemy>>& GameModel::getEnemies() {
    return m_enemies;
}

std::vector<std::unique_ptr<Coin>>& GameModel::getCoins() {
    return m_coins;
}