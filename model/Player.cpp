#include "Player.h"

void Player::setLives(int lives) {
    this->m_lives = lives;
}

double Player::getVelocityY() const { 
    return m_velocityY;
}

double Player::getVelocityX() const {
    return m_velocityX; 
}

bool Player::getIsInvulnerable() const { 
    return m_isInvulnerable; 
}

void Player::jump() {
    if (!m_isJumping) {
        m_velocityY = -12;
        m_isJumping = true;
    }
}

void Player::moveLeft() {
    m_velocityX = -15;
}
 
void Player::moveRight() {
    m_velocityX = 15;
}

void Player::stop() {
    m_velocityX *= 0.8;
}

int Player::getLives() const { 
    return m_lives;
}

int Player::getCoins() const { 
    return m_coins;
}

void Player::addCoin() {
    m_coins++;
}

void Player::takeDamage() {
    if (m_isInvulnerable) {
        return;
    }

    m_lives--;
    setInvulnerable(true, 2.0f);

    if (m_lives > 0) {
        respawn();
    }
    else {
        setActive(false);
    }
}

void Player::respawn() {
    m_x = 100;
    m_y = 300;
    m_velocityY = 0;
    m_velocityX = 0;
    m_isJumping = false;
}

void Player::stopJump() {
    if (m_velocityY < 0) m_velocityY = 0;
    m_isJumping = false;
}

void Player::setInvulnerable(bool invuln, float duration) {
    m_isInvulnerable = invuln;
    if (invuln) {
        m_invulnerabilityTimer = duration;
    }
    else {
        m_invulnerabilityTimer = 0.0f;
    }
}

void Player::update() {
    if (m_isInvulnerable) {
        m_invulnerabilityTimer -= 1.0f / 60.0f;
        if (m_invulnerabilityTimer <= 0.0f) {
            setInvulnerable(false);
        }
    }

    m_velocityY += m_gravity;

    m_x += m_velocityX;
    m_y += m_velocityY;

    if (m_velocityY > 15) m_velocityY = 15;

    if (m_y > 700) {
        takeDamage();
        if (m_lives > 0) {
            setPosition(100, 300);
            m_velocityX = 0;
            m_velocityY = 0;
            m_isJumping = false;
        }
    }
}

void Player::reset() {
    setPosition(100, 300);
    m_velocityX = 0;
    m_velocityY = 0;
    m_isJumping = false;
    m_lives = 1;
    m_coins = 0;
    m_isInvulnerable = false;
    m_invulnerabilityTimer = 0.0f;
    setActive(true);
}