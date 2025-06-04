#pragma once
#include "../model/GameModel.h"
#include "MenuController.h"
#include "../view/GameView.h"
#include <FL/Fl.H>
#include <memory>

class MenuController;
class GameView;

class GameController {
private:
    std::unique_ptr<GameModel> m_model;
    std::unique_ptr<GameView> m_view;
    MenuController* m_menuController;
    bool m_isGameActive;

    bool m_keyLeft = false;
    bool m_keyRight = false;
    bool m_keySpace = false;

    static void timerCallback(void* data);

    void processInput();

    void cleanUp();

public:
    GameController(int width, int height);
    ~GameController();

    void run();
    void restart();

    void setMenuController(MenuController* menu);
    void returnToMenu();

    void handleKeyDown(int key);
    void handleKeyUp(int key);
};
