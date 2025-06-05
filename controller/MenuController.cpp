#include "MenuController.h"

MenuController::MenuController() : m_menuView{ std::make_unique<MenuView>(400, 300, "Mario Game - Gusev") },
m_gameController{ std::make_unique<GameController>(1200, 600) },
m_aboutController{ std::make_unique<AboutController>() } {
    m_gameController->setMenuController(this);
}

void MenuController::setupCallBacks()
{
    m_menuView->setCallBackNewGame([this]() {
        m_menuView->hide();
        m_gameController->run();
        });

    m_menuView->setCallBackAbout([this]() {
        m_aboutController->run();
        });

    m_menuView->setCallBackExit([this]() {
        exit(0);
        });
}

void MenuController::run() {
    setupCallBacks();
    m_menuView->show();
}