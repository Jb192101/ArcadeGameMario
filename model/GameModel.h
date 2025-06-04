#pragma once

#include <vector>
#include <memory>
#include <random>
#include <thread>
#include <atomic>
#include <chrono>
#include <algorithm>
#include <mutex>
#include "Player.h"
#include "Platform.h"
#include "Enemy.h"
#include "Coin.h"
#include <functional>

class GameModel {
private:
    std::unique_ptr<Player> m_player;
    std::vector<std::unique_ptr<Platform>> m_platforms;
    std::vector<std::unique_ptr<Enemy>> m_enemies;
    std::vector<std::unique_ptr<Coin>> m_coins;

    double m_cameraX;
    double m_worldWidth;
    std::atomic<bool> m_isGameOver{ false };
    std::atomic<bool> m_isRunning{ false };

    std::mt19937 m_rng;
    std::uniform_real_distribution<double> m_dist;

    void generateWorldSegment(double startX, double endX);
    void clearAllObjects();

    std::thread m_updateThread;
    std::mutex m_dataMutex;

public:
    GameModel();
    ~GameModel();

    Player* getPlayer();
    std::vector<std::unique_ptr<Platform>>& getPlatforms();
    std::vector<std::unique_ptr<Enemy>>& getEnemies();
    std::vector<std::unique_ptr<Coin>>& getCoins();
    double getCameraX() const;
    bool getGameOver() const;
    bool isGameRunning() const;

    void withDataLock(std::function<void()> func) {
        std::lock_guard<std::mutex> lock(m_dataMutex);
        func();
    }

    void start();
    void stop();
    void reset();
    void update();
    void checkCollisions();
    void generateMoreWorld();
};