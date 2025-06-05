#pragma once

#include <memory>
#include "../view/AboutWindow.h"
#include "MenuController.h"

class MenuController;

class AboutController {
private:
	std::unique_ptr<AboutWindow> m_aboutWindow = nullptr;
	MenuController* m_menuController;

public:
	AboutController();
	void run();

	void setMenuController(MenuController* menu);
};