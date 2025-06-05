#pragma once

#include "GameController.h"
#include "AboutController.h"
#include "../view/MenuView.h"
#include <memory>

class GameController;
class AboutController;

class MenuController {
private:
    std::unique_ptr<MenuView> m_menuView;
    std::unique_ptr<GameController> m_gameController;
    std::unique_ptr<AboutController> m_aboutController;

public:
    MenuController();

    void setupCallBacks();

    void run();
};
