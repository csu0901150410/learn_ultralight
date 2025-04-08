#pragma once

#include <AppCore/AppCore.h>

#include <opencv2/opencv.hpp>

#include "lsWindow.h"

using namespace ultralight;

struct lsSubWindow
{
    std::unique_ptr<lsWindow> m_window;
    RefPtr<View> m_view;
    RefPtr<Buffer> m_surface;
    std::unique_ptr<sf::Texture> m_texture;
    std::unique_ptr<sf::Sprite> m_sprite;
};

class lsApp : public lsWindowListener, public ViewListener, public LoadListener
{
public:
    lsApp();
    ~lsApp();

    void run();

    // 从 LoadListener 继承的方法
    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const String& url) override;
    virtual void OnDOMReady(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const String& url) override;

    // 从 ViewListener 继承的方法
    virtual void OnChangeCursor(ultralight::View* caller, Cursor cursor) override;

    // 从 lsWindowListener 继承的方法
    virtual void OnResize(lsWindow *window, uint32_t width, uint32_t height) override;
    virtual void OnMouseEvent(lsWindow *window, const ultralight::MouseEvent& event) override;
    virtual void OnKeyEvent(lsWindow *window, const ultralight::KeyEvent& event) override;

    JSValue js_open_window(const JSObject &thisObject, const JSArgs &args);

    JSValue script_call_native(const JSObject &thisObject, const JSArgs &args);

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void processCvFrame();

protected:
    std::unique_ptr<lsWindow> window_;// 主窗口
    int width_;
    int height_;

    RefPtr<Renderer> renderer_;
    RefPtr<View> view_;

    cv::Mat rgbmat_;

    RefPtr<Buffer> gui_buffer_;

    std::unique_ptr<sf::Texture> gui_texture_;
    std::unique_ptr<sf::Texture> canvas_texture_;
    std::unique_ptr<sf::Sprite> gui_sprite_;
    std::unique_ptr<sf::Sprite> canvas_sprite_;

    cv::VideoCapture camera_;

    sf::Time time_per_frame_;

private:
    std::vector<lsSubWindow> m_subwindows;

public:
    void create_subwindow(const std::string& url);
};
