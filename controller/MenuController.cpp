#include "MenuController.h"
#include <FL/fl_message.H>
#include <iostream>

MenuController::MenuController() : m_menuView{ std::make_unique<MenuView>(400, 300, "Mario Game - Gusev") } {
    m_gameController = std::make_unique<GameController>(1200, 600);
    m_gameController->setMenuController(this);
}

void MenuController::setupCallBacks()
{
    m_menuView->setCallBackNewGame([this]() {
        m_menuView->hide();
        m_gameController->run();
        });

    m_menuView->setCallBackAbout([this]() {
        this->showInstructions();
        });

    m_menuView->setCallBackExit([this]() {
        exit(0);
        });
}

void MenuController::run() {
    setupCallBacks();
    m_menuView->show();
}

void MenuController::showInstructions()
{
    fl_message("Mario\nHandle control:\nLeft - moving left\nRight - moving right\nSpace - jumping");
}