#include "GameController.h"

GameController::GameController(int width, int height)
    : m_isGameActive{ false }, m_menuController{ nullptr } {
    m_model = std::make_unique<GameModel>();

    m_view = std::make_unique<GameView>(width, height, "Mario", m_model.get());
    m_view->user_data(this);
}

GameController::~GameController() {
    cleanUp();
}

void GameController::cleanUp() {
    Fl::remove_timeout(timerCallback, this);

    if (m_isGameActive && m_model) {
        m_model->stop();
    }

    m_isGameActive = false;

    if (m_view) {
        m_view->hide();
    }
}

void GameController::run() {
    if (!m_model || !m_view) {
        return;
    }

    cleanUp();

    m_isGameActive = true;
    m_view->show();

    m_model->reset();
    m_model->start();

    m_keyLeft = false;
    m_keyRight = false;
    m_keySpace = false;

    Fl::add_timeout(1.0 / 60.0, timerCallback, this);
}

void GameController::returnToMenu() {
    cleanUp();

    if (m_menuController) {
        m_menuController->run();
    }
}

void GameController::timerCallback(void* data) {
    GameController* controller = static_cast<GameController*>(data);

    if (!controller) {
        return;
    }

    if (!controller->m_model) {
        return;
    }

    if (controller->m_isGameActive) {
        if (controller->m_model->getGameOver()) {
            controller->returnToMenu();
        }
        else {
            controller->processInput();
            Fl::repeat_timeout(1.0 / 60.0, timerCallback, data);
        }
    }
}

void GameController::handleKeyDown(int key) {
    switch (key) {
    case FL_Left: m_keyLeft = true; break;
    case FL_Right: m_keyRight = true; break;
    case ' ': m_keySpace = true; break;
    }
}

void GameController::handleKeyUp(int key) {
    switch (key) {
    case FL_Left: m_keyLeft = false; break;
    case FL_Right: m_keyRight = false; break;
    case ' ': m_keySpace = false; break;
    }
}

void GameController::processInput() {
    if (!m_model || !m_model->getPlayer()) return;

    m_model->getPlayer()->stop();

    if (m_keyLeft) m_model->getPlayer()->moveLeft();
    if (m_keyRight) m_model->getPlayer()->moveRight();
    if (m_keySpace) m_model->getPlayer()->jump();
}

void GameController::setMenuController(MenuController* menu) {
    m_menuController = menu;
}