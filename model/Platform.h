#pragma once
#include "GameObject.h"

class Platform : public GameObject {
public:
    Platform(double x, double y, double w, double h)
        : GameObject{ x, y, w, h } {}

    void update() override;
};
