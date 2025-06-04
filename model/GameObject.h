#pragma once
#include <vector>
#include <memory>

class GameObject {
protected:
    double m_x, m_y;
    double m_width, m_height;
    bool m_isActive;

public:
    GameObject(double x, double y, double w, double h)
        : m_x{ x }, m_y{ y }, m_width{ w }, m_height{ h }, m_isActive{ true } {}

    GameObject(const GameObject& other)
        : m_x{ other.m_x }, m_y{ other.m_y }, m_width{ other.m_width }, m_height{ other.m_height }, m_isActive{ other.m_isActive } {}

    virtual ~GameObject() = default;

    double getX() const;
    double getY() const;
    double getWidth() const;
    double getHeight() const;
    bool getActive() const;

    void setPosition(double nx, double ny);
    void setSize(double w, double h);
    void setActive(bool active);

    virtual void update() = 0;

    bool collidesWith(const GameObject& other) const;
};
