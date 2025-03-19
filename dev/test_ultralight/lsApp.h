#pragma once

#include <AppCore/AppCore.h>

#include <opencv2/opencv.hpp>

#include "lsWindow.h"

using namespace ultralight;

class lsApp : public lsWindowListener, public ViewListener, public LoadListener
{
public:
    lsApp();
    ~lsApp();

    void run();

    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const String& url) override;

    virtual void OnResize(uint32_t width, uint32_t height) override;

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
};
