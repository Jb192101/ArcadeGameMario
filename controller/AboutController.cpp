#include "AboutController.h"

AboutController::AboutController() : m_aboutWindow{ std::make_unique<AboutWindow>(400, 400, "About Mario Game") } {
}

void AboutController::run() {
	m_aboutWindow->show();
}

void AboutController::setMenuController(MenuController* menu) {
	m_menuController = menu;
}