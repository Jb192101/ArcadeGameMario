#pragma once
#include "GameObject.h"

class Player : public GameObject {
private:
    double m_velocityX, m_velocityY;
    double m_gravity;
    bool m_isJumping;
    int m_lives;
    int m_coins;
    bool m_isInvulnerable;
    float m_invulnerabilityTimer;

public:
    Player(double x, double y)
        : GameObject{ x, y, 30, 50 }, m_velocityX{ 0 }, m_velocityY{ 0 },
        m_gravity{ 0.5 }, m_isJumping{ true }, m_lives{ 1 }, m_coins{ 0 },
        m_isInvulnerable{ false }, m_invulnerabilityTimer{ 0.0f } {
    }

    void jump();
    void moveLeft();
    void moveRight();
    void stop();

    int getLives() const;
    int getCoins() const;
    void addCoin();
    void takeDamage();
    void respawn();
    void setLives(int lives);
    void stopJump();

    void update() override;

    bool getIsInvulnerable() const;
    void setInvulnerable(bool invuln, float duration = 2.0f);

    double getVelocityY() const;
    double getVelocityX() const;

    void reset();
};