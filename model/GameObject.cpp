#include "GameObject.h"

double GameObject::getX() const { 
    return m_x; 
}

double GameObject::getY() const { 
    return m_y; 
}

double GameObject::getWidth() const { 
    return m_width; 
}

double GameObject::getHeight() const { 
    return m_height; 
}

bool GameObject::getActive() const {
    return m_isActive;
}

void GameObject::setPosition(double nx, double ny) { 
    m_x = nx; 
    m_y = ny;
}

void GameObject::setSize(double w, double h) {
    m_width = w;
    m_height = h;
}

void GameObject::setActive(bool active) {
    m_isActive = active; 
}

bool GameObject::collidesWith(const GameObject& other) const {
    return m_x < other.m_x + other.m_width &&
        m_x + m_width > other.m_x &&
        m_y < other.m_y + other.m_height &&
        m_y + m_height > other.m_y;
}
