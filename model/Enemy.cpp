#include "Enemy.h"
#include <iostream>

void Enemy::update() {
    if (m_movingRight) {
        m_x += m_speed;
    }
    else {
        m_x -= m_speed;
    }
}

void Enemy::applyGravity(const std::vector<std::unique_ptr<Platform>>& platforms) {
    if (!m_isOnGround) {
        m_velocityY += m_gravity;
    }

    m_y += m_velocityY;

    m_isOnGround = false;
    for (const auto& platform : platforms) {
        if (platform && platform->getActive() && collidesWith(*platform)) {
            if (m_velocityY > 0 && m_y < platform->getY()) {
                m_y = platform->getY() - m_height;
                m_velocityY = 0;
                m_isOnGround = true;

                checkPlatformEdge(*platform);
                break;
            }
        }
    }

    if (m_y > 700) {
        setActive(false);
    }
}

void Enemy::checkPlatformEdge(const Platform& platform) {
    double platformLeft = platform.getX();
    double platformRight = platform.getX() + platform.getWidth();
    double enemyCenter = m_x + m_width / 2;

    if (m_movingRight && enemyCenter >= platformRight - 10) {
        m_movingRight = false;
    }
    else if (!m_movingRight && enemyCenter <= platformLeft + 10) {
        m_movingRight = true;
    }
}

double Enemy::getSpeed() const {
    return m_speed;
}

bool Enemy::isMovingRight() const { 
    return m_movingRight;
}

bool Enemy::isGrounded() const { 
    return m_isOnGround;
}

double Enemy::getVelocityY() const { 
    return m_velocityY;
}