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

void lsWindow::OnMouseEvent(const sf::Event &event) {
    // 把sfml的鼠标事件转换成ultralight事件传递给监听者
    if (sf::Event::MouseMoved != event.type 
        && sf::Event::MouseButtonPressed != event.type 
        && sf::Event::MouseButtonReleased != event.type)
        return;

    // 根据sfml鼠标事件结构体的数据填充ultralight的鼠标事件结构体
    ultralight::MouseEvent evt;

    // 鼠标移动、按下、抬起都可以取位置
    evt.x = event.mouseMove.x;
    evt.y = event.mouseMove.y;

    if (event.type == sf::Event::MouseMoved) {
        evt.type = ultralight::MouseEvent::kType_MouseMoved;
        evt.button = ultralight::MouseEvent::kButton_None;
    }
    else {
        // 鼠标左中右哪个按键按下
        switch (event.mouseButton.button) {
        case sf::Mouse::Left:
            evt.button = ultralight::MouseEvent::kButton_Left;
        break;

        case sf::Mouse::Right:
            evt.button = ultralight::MouseEvent::kButton_Right;
        break;

        case sf::Mouse::Middle:
            evt.button = ultralight::MouseEvent::kButton_Middle;
        break;

        default:
            evt.button = ultralight::MouseEvent::kButton_None;
        }

        // 鼠标按键是按下还是抬起
        if (event.type == sf::Event::MouseButtonPressed) {
            evt.type = ultralight::MouseEvent::kType_MouseUp;
        }
        else {
            evt.type = ultralight::MouseEvent::kType_MouseDown;
        }
    }

    // 包装好ultralight事件结构体后传递给监听者
    listener_->OnMouseEvent(evt);
}
