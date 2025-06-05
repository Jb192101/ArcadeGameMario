#include "MenuView.h"

MenuView::MenuView(int w, int h, const char* title)
    : Fl_Window{ w, h, title }, m_newGameBtn{ std::make_unique<Fl_Button>(w / 2 - 100, h / 2 - 60, 200, 40, "New Game") },
    m_aboutBtn{ std::make_unique<Fl_Button>(w / 2 - 100, h / 2, 200, 40, "About") },
    m_exitBtn{ std::make_unique<Fl_Button>(w / 2 - 100, h / 2 + 60, 200, 40, "Exit") } {
    begin();

    m_newGameBtn->callback([](Fl_Widget* w, void* data) {
            MenuView* view = static_cast<MenuView*>(data);
            if (view->m_startGameCallback) {
                view->m_startGameCallback();
            }
        }
    , this);

    m_aboutBtn->callback([](Fl_Widget* w, void* data) {
            MenuView* view = static_cast<MenuView*>(data);
            if (view->m_aboutCallback) {
                view->m_aboutCallback();
            }
        }
    , this);

    m_exitBtn->callback([](Fl_Widget* w, void* data) {
            MenuView* view = static_cast<MenuView*>(data);
            if (view->m_exitCallback) {
                view->m_exitCallback();
            }
        }
    , this);

    end();
}

void MenuView::setCallBackNewGame(std::function<void()> callback) {
    m_startGameCallback = callback;
}

void MenuView::setCallBackAbout(std::function<void()> callback) {
    m_aboutCallback = callback;
}

void MenuView::setCallBackExit(std::function<void()> callback) {
    m_exitCallback = callback;
}
