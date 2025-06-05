#pragma once

#include <FL/Fl_Double_Window.H>
#include <FL/fl_draw.H>
#include <FL/Fl.H>
#include "../model/GameModel.h"
#include "../controller/GameController.h"

class GameView : public Fl_Double_Window {
private:
    GameModel* m_model;

public:
    GameView(int width, int height, const char* title, GameModel* model)
        : Fl_Double_Window{ width, height, title }, m_model{ model } {
        Fl::add_timeout(1.0 / 60.0, timerCallback, this);
    }

    void draw() override;

    static void timerCallback(void* data);

private:
    int handle(int event) override;

    void drawPlatforms();
    void drawPlatform(double x, double y, double width, double height);
    void drawBackground();
    void drawClouds();
    void drawCloud(double x, double y, double size);
    void drawEnemies();
    void drawEnemy(double x, double y, double width, double height);    
    void drawCoins();
    void drawCoin(double x, double y, double width, double height, double rotation);
    void drawPlayer();
    void drawPlayerCharacter(double x, double y, double width, double height);
    void drawUI();
};