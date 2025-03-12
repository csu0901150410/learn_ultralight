#include "lsWindow.h"

lsWindow::lsWindow(uint32_t width, uint32_t height)
{
    window_ = new sf::RenderWindow(sf::VideoMode({ width, height }), "sfml window");
    window_->setVerticalSyncEnabled(true);// 开启垂直同步

    width_ = width;
    height_ = height;

    // SFML窗口事件传递到ultralight
}

lsWindow::~lsWindow()
{
}

uint32_t lsWindow::width() const {
    sf::Vector2u size = window_->getSize();
    return size.x;
}

uint32_t lsWindow::height() const {
    sf::Vector2u size = window_->getSize();
    return size.y;
}

void lsWindow::Close() {
    window_->close();
}

void lsWindow::PresentFrame()
{
    window_->display();
}

void lsWindow::OnResize(uint32_t width, uint32_t height) {
    listener_->OnResize(width, height);
}
