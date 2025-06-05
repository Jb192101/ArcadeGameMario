#pragma once

#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Box.H>
#include <string>
#include <vector>
#include <functional>

class AboutWindow : public Fl_Double_Window {
private:
    Fl_Button* m_okButton;
    std::vector<std::string> m_controlLines;
    std::function<void()> m_closeCallback;

    static void onOkButtonClicked(Fl_Widget* widget, void* data);

public:
    AboutWindow(int width, int height, const char* title);
    ~AboutWindow();

    void draw() override;
    void setCloseCallback(std::function<void()> callback);
};