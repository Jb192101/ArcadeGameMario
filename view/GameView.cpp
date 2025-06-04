#include "GameView.h"

void GameView::timerCallback(void* data) {
    GameView* view = static_cast<GameView*>(data);
    if (view) {
        view->redraw();
        Fl::repeat_timeout(1.0 / 60.0, timerCallback, data);
    }
}

int GameView::handle(int event) {
    GameController* controller = static_cast<GameController*>(user_data());

    if (!controller) {
        return Fl_Double_Window::handle(event);
    }

    switch (event) {
    case FL_KEYDOWN:
        controller->handleKeyDown(Fl::event_key());
        return 1;
    case FL_KEYUP:
        controller->handleKeyUp(Fl::event_key());
        return 1;
    case FL_FOCUS:
        return 1;
    default:
        return Fl_Double_Window::handle(event);
    }
}

void GameView::draw() {
    Fl_Double_Window::draw();

    if (!m_model) {
        return;
    }

    fl_color(FL_CYAN);
    fl_rectf(0, 0, w(), h());

    try {
        m_model->withDataLock([this]() {
            drawPlatforms();
            drawCoins();
            drawEnemies();
            drawPlayer();
            });

        drawUI();
    }
    catch (const std::exception& e) {
    }
}

void GameView::drawBackground() {
    for (int y = 0; y < h(); y++) {
        double ratio = (double)y / h();
        int r = (int) (135 + ratio * (173 - 135));
        int g = (int) (206 + ratio * (216 - 206));
        int b = (int) (235 + ratio * (230 - 235));
        fl_color(r, g, b);
        fl_line(0, y, w(), y);
    }

    drawClouds();
}

void GameView::drawClouds() {
    fl_color(255, 255, 255);

    double cameraOffset = m_model->getCameraX() * 0.3;

    drawCloud(200 - cameraOffset, 80, 60);
    drawCloud(500 - cameraOffset, 120, 80);
    drawCloud(800 - cameraOffset, 60, 70);
    drawCloud(1200 - cameraOffset, 100, 90);
}

void GameView::drawCloud(double x, double y, double size) {
    if (x + size >= 0 && x - size <= w()) {
        fl_color(255, 255, 255);
        fl_pie(x - size / 2, y, size * 0.6, size * 0.4, 0, 360);
        fl_pie(x - size / 4, y - size / 6, size * 0.8, size * 0.5, 0, 360);
        fl_pie(x + size / 4, y - size / 8, size * 0.7, size * 0.45, 0, 360);
        fl_pie(x, y + size / 8, size * 0.9, size * 0.3, 0, 360);
    }
}

void GameView::drawPlatforms() {
    auto& platforms = m_model->getPlatforms();

    for (auto& platform : platforms) {
        if (platform && platform->getActive()) {
            double x = platform->getX() - m_model->getCameraX();
            if (x + platform->getWidth() >= 0 && x <= w()) {
                drawPlatform(x, platform->getY(), platform->getWidth(), platform->getHeight());
            }
        }
    }
}

void GameView::drawPlatform(double x, double y, double width, double height) {
    fl_color(139, 69, 19);
    fl_rectf(x, y, width, height);

    fl_color(34, 139, 34);
    fl_rectf(x, y - 5, width, 5);

    fl_color(0, 100, 0);
    for (int i = 0; i < width; i += 8) {
        fl_line(x + i, y - 5, x + i + 2, y - 2);
        fl_line(x + i + 4, y - 4, x + i + 6, y - 1);
    }

    fl_color(101, 67, 33);
    fl_line(x, y, x, y + height);
    fl_line(x + width, y, x + width, y + height);

    fl_color(160, 82, 45);
    for (int i = 5; i < width - 5; i += 15) {
        for (int j = 5; j < height - 5; j += 10) {
            fl_rectf(x + i, y + j, 3, 2);
        }
    }
}

void GameView::drawEnemies() {
    auto& enemies = m_model->getEnemies();

    for (auto& enemy : enemies) {
        if (enemy && enemy->getActive()) {
            double screenX = enemy->getX() - m_model->getCameraX();
            double screenY = enemy->getY();

            if (screenX + enemy->getWidth() >= -50 && screenX <= w() + 50) {
                drawEnemy(screenX, screenY, enemy->getWidth(), enemy->getHeight());
            }
        }
    }
}

void GameView::drawEnemy(double x, double y, double width, double height) {
    fl_color(220, 20, 60);
    fl_pie(x, y + height * 0.3, width, height * 0.7, 0, 360);

    fl_color(255, 69, 0);
    fl_pie(x + width * 0.1, y, width * 0.8, height * 0.6, 0, 360);

    fl_color(255, 255, 255);
    fl_pie(x + width * 0.2, y + height * 0.15, width * 0.15, height * 0.15, 0, 360);
    fl_pie(x + width * 0.65, y + height * 0.15, width * 0.15, height * 0.15, 0, 360);

    fl_color(0, 0, 0);
    fl_pie(x + width * 0.23, y + height * 0.18, width * 0.08, height * 0.08, 0, 360);
    fl_pie(x + width * 0.68, y + height * 0.18, width * 0.08, height * 0.08, 0, 360);

    fl_color(0, 0, 0);
    fl_arc(x + width * 0.3, y + height * 0.35, width * 0.4, height * 0.2, 0, 180);

    fl_color(139, 69, 19);
    fl_rectf(x + width * 0.2, y + height * 0.85, width * 0.15, height * 0.15);
    fl_rectf(x + width * 0.65, y + height * 0.85, width * 0.15, height * 0.15);
}

void GameView::drawCoins() {
    auto& coins = m_model->getCoins();
    static double coinRotation = 0;
    coinRotation += 3;

    for (auto& coin : coins) {
        if (coin && coin->getActive()) {
            double x = coin->getX() - m_model->getCameraX();
            if (x + coin->getWidth() >= 0 && x <= w()) {
                drawCoin(x, coin->getY(), coin->getWidth(), coin->getHeight(), coinRotation);
            }
        }
    }
}

void GameView::drawCoin(double x, double y, double width, double height, double rotation) {
    fl_color(255, 215, 0);
    fl_pie(x, y, width, height, 0, 360);

    fl_color(255, 255, 0);
    fl_pie(x + width * 0.15, y + height * 0.15, width * 0.7, height * 0.7, 0, 360);

    fl_color(255, 140, 0);
    double centerX = x + width / 2;
    double centerY = y + height / 2;

    fl_line(centerX - width * 0.2, centerY, centerX + width * 0.2, centerY);
    fl_line(centerX, centerY - height * 0.2, centerX, centerY + height * 0.2);
    fl_line(centerX - width * 0.15, centerY - height * 0.15, centerX + width * 0.15, centerY + height * 0.15);
    fl_line(centerX + width * 0.15, centerY - height * 0.15, centerX - width * 0.15, centerY + height * 0.15);

    fl_color(255, 255, 255);
    fl_pie(x + width * 0.2, y + height * 0.1, width * 0.3, height * 0.3, 0, 360);
}

void GameView::drawPlayer() {
    try {
        if (m_model->getPlayer() && m_model->getPlayer()->getActive()) {
            double x = m_model->getPlayer()->getX() - m_model->getCameraX();
            double y = m_model->getPlayer()->getY();
            double width = m_model->getPlayer()->getWidth();
            double height = m_model->getPlayer()->getHeight();

            drawPlayerCharacter(x, y, width, height);
        }
    }
    catch (const std::exception& e) {
    }
}

void GameView::drawPlayerCharacter(double x, double y, double width, double height) {
    bool isInvulnerable = static_cast<Player*>(m_model->getPlayer())->getIsInvulnerable();
    static int blinkCounter = 0;
    blinkCounter++;

    if (isInvulnerable && (blinkCounter / 10) % 2 == 0) {
        return;
    }

    fl_color(0, 100, 200);
    fl_pie(x, y + height * 0.4, width, height * 0.6, 0, 360);

    fl_color(255, 220, 177);
    fl_pie(x + width * 0.1, y, width * 0.8, height * 0.5, 0, 360);

    fl_color(255, 0, 0);
    fl_pie(x + width * 0.05, y - height * 0.1, width * 0.9, height * 0.4, 0, 360);

    fl_color(200, 0, 0);
    fl_pie(x - width * 0.1, y + height * 0.1, width * 0.6, height * 0.2, 0, 180);

    fl_color(0, 0, 0);
    fl_pie(x + width * 0.25, y + height * 0.2, width * 0.1, height * 0.1, 0, 360);
    fl_pie(x + width * 0.65, y + height * 0.2, width * 0.1, height * 0.1, 0, 360);

    fl_color(255, 200, 150);
    fl_pie(x + width * 0.45, y + height * 0.3, width * 0.1, height * 0.08, 0, 360);

    fl_color(139, 69, 19);
    fl_rectf(x + width * 0.3, y + height * 0.35, width * 0.4, height * 0.05);

    fl_color(255, 220, 177);
    fl_pie(x - width * 0.2, y + height * 0.5, width * 0.3, height * 0.2, 0, 360);
    fl_pie(x + width * 0.9, y + height * 0.5, width * 0.3, height * 0.2, 0, 360);

    fl_color(0, 0, 139);
    fl_rectf(x + width * 0.2, y + height * 0.85, width * 0.25, height * 0.15);
    fl_rectf(x + width * 0.55, y + height * 0.85, width * 0.25, height * 0.15);

    fl_color(139, 69, 19);
    fl_pie(x + width * 0.1, y + height * 0.95, width * 0.35, height * 0.1, 0, 360);
    fl_pie(x + width * 0.55, y + height * 0.95, width * 0.35, height * 0.1, 0, 360);
}

void GameView::drawUI() {
    try {
        m_model->withDataLock([this]() {
            if (!m_model->getPlayer()) return;

            fl_color(0, 0, 0);
            fl_rectf(10, 10, 300, 120);

            fl_color(255, 255, 255);
            fl_rect(10, 10, 300, 120);

            fl_color(255, 255, 255);
            fl_font(FL_HELVETICA_BOLD, 18);

            fl_draw("Coins:", 20, 35);
            fl_color(255, 215, 0);
            fl_pie(90, 20, 20, 20, 0, 360);
            fl_color(255, 255, 255);
            fl_draw(std::to_string(m_model->getPlayer()->getCoins()).c_str(), 120, 35);
            });

        if (m_model->getGameOver()) {
            fl_font(FL_HELVETICA_BOLD, 32);
            fl_color(FL_RED);
            fl_draw("GAME OVER", w() / 2 - 80, h() / 2);
            fl_font(FL_HELVETICA, 16);
        }
    }
    catch (const std::exception& e) {
    }
}