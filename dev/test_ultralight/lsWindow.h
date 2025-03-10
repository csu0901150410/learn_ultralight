#pragma once
#include <Ultralight/Ultralight.h>

#include <AppCore/AppCore.h>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

using namespace ultralight;

class lsWindow
{
public:
    lsWindow(uint32_t width, uint32_t height);
    ~lsWindow();

    void set_listener(WindowListener *listener) { listener_ = listener; }
    WindowListener *get_listener() { return listener_; }

    uint32_t width() const;
    uint32_t height() const;

    void Close();

    void PresentFrame();

    sf::RenderWindow *get_handle() { return window_; }

protected:
    sf::RenderWindow *window_;// sfml窗口
    WindowListener *listener_;// appcore的窗口监听器

    uint32_t width_;
    uint32_t height_;
};
