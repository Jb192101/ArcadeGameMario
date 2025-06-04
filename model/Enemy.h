#pragma once
#include "GameObject.h"
#include "Platform.h"

class Enemy : public GameObject {
private:
    double m_speed;
    bool m_movingRight;
    double m_gravity;
    double m_velocityY;
    bool m_isOnGround;

    void checkPlatformEdge(const Platform& platform);

public:
    Enemy(double x, double y, double speed, bool movingRight)
        : GameObject{ x, y, 40, 40 }, m_speed{ speed }, m_movingRight{ movingRight },
        m_gravity{ 0.5 }, m_velocityY{ 0 }, m_isOnGround{ false } {
    }

    Enemy(const Enemy& other)
        : GameObject{ other }, m_speed{ other.m_speed }, m_movingRight{ other.m_movingRight },
        m_gravity{ other.m_gravity }, m_velocityY{ other.m_velocityY }, m_isOnGround{ other.m_isOnGround } {}

    void update() override;
    void applyGravity(const std::vector<std::unique_ptr<Platform>>& platforms);

    double getSpeed() const;
    bool isMovingRight() const;
    bool isGrounded() const;
    double getVelocityY() const;
};