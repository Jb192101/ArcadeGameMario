#pragma once
#include "GameObject.h"

class Coin : public GameObject {
public:
    Coin(double x, double y)
        : GameObject{ x, y, 20, 20 } {}

    void update() override;
};
