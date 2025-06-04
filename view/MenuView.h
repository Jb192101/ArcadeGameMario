#pragma once
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Button.H>
#include <memory>
#include <functional>

class MenuView : public Fl_Window {
private:
    std::unique_ptr<Fl_Button> m_newGameBtn = nullptr;
    std::unique_ptr<Fl_Button> m_aboutBtn = nullptr;
    std::unique_ptr<Fl_Button> m_exitBtn = nullptr;

    std::function<void()> m_startGameCallback;
    std::function<void()> m_aboutCallback;
    std::function<void()> m_exitCallback;

public:
    MenuView(int w, int h, const char* title);

    void setCallBackNewGame(std::function<void()> callback);
    void setCallBackAbout(std::function<void()> callback);
    void setCallBackExit(std::function<void()> callback);
};
