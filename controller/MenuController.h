#pragma once

#include "GameController.h"
#include "../view/MenuView.h"
#include <memory>

class GameController;

class MenuController {
private:
    std::unique_ptr<MenuView> m_menuView;
    std::unique_ptr<GameController> m_gameController;

public:
    MenuController();

    void setupCallBacks();

    void showInstructions();

    void run();
};
