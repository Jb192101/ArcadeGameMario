#include "AboutWindow.h"

AboutWindow::AboutWindow(int width, int height, const char* title)
    : Fl_Double_Window(width, height, title) {
    color(FL_GRAY);
    begin();

    m_controlLines = {
        "Game Controls:",
        "",
        "Left Arrow - Move Left",
        "Right Arrow - Move Right",
        "Space - Jump",
        ""
    };

    m_okButton = new Fl_Button(width / 2 - 50, height - 50, 100, 30, "OK");
    m_okButton->callback(onOkButtonClicked, this);
    m_okButton->color(FL_LIGHT2);

    end();

    position((Fl::w() - width) / 2, (Fl::h() - height) / 2);
}

AboutWindow::~AboutWindow() {
}

void AboutWindow::draw() {
    Fl_Double_Window::draw();

    fl_color(FL_BLUE);
    fl_font(FL_HELVETICA_BOLD, 24);
    fl_draw("About Mario Game", 20, 40);

    fl_line_style(FL_SOLID, 2);
    fl_line(20, 50, w() - 20, 50);
    fl_line_style(0);

    fl_color(FL_BLACK);
    fl_font(FL_HELVETICA, 16);

    int y = 80;
    for (const auto& line : m_controlLines) {
        fl_draw(line.c_str(), 30, y);
        y += 20;
    }

    fl_color(FL_GRAY);
    fl_rect(0, 0, w(), h());
}

void AboutWindow::onOkButtonClicked(Fl_Widget* widget, void* data) {
    AboutWindow* window = static_cast<AboutWindow*>(data);
    if (window) {
        if (window->m_closeCallback) {
            window->m_closeCallback();
        }
        window->hide();
    }
}

void AboutWindow::setCloseCallback(std::function<void()> callback) {
    m_closeCallback = callback;
}