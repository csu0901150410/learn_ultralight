#pragma once

#include <unordered_map>

#include <Ultralight/Ultralight.h>
#include <AppCore/AppCore.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace ultralight;

class lsWindow;

class lsWindowListener {
public:
    virtual ~lsWindowListener() {}

    // virtual void OnClose() = 0;
    virtual void OnResize(lsWindow *window, uint32_t width, uint32_t height) = 0;
    // virtual void OnChangeFocus(bool focused) = 0;
    virtual void OnKeyEvent(lsWindow *window, const ultralight::KeyEvent &evt) = 0;
    virtual void OnMouseEvent(lsWindow *window, const ultralight::MouseEvent& evt) = 0;
    // virtual void OnScrollEvent(const ScrollEvent& evt) = 0;
};

class lsWindow
{
public:
    lsWindow(uint32_t width, uint32_t height);
    ~lsWindow();

    void set_listener(lsWindowListener *listener) { listener_ = listener; }
    lsWindowListener *get_listener() { return listener_; }

    uint32_t width() const;
    uint32_t height() const;

    void Close();

    void SetCursor(ultralight::Cursor cursor);

    void PresentFrame();

    sf::RenderWindow *get_handle() { return window_; }

    void OnResize(uint32_t width, uint32_t height);

    void OnMouseEvent(const sf::Event &event);

    void OnKeyEvent(const sf::Event &event);

protected:
    sf::RenderWindow *window_;// sfml窗口
    lsWindowListener *listener_;// appcore的窗口监听器

    uint32_t width_;
    uint32_t height_;

    // 存储sfml中的各种光标实例指针
    std::unordered_map<ultralight::Cursor, sf::Cursor *> cursor_map_;
};
