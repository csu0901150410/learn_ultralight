#include "lsApp.h"

#include <chrono>

lsApp::lsApp() 
    : gui_texture_(new sf::Texture())
    , canvas_texture_(new sf::Texture()) 
    , gui_sprite_(new sf::Sprite())
    , canvas_sprite_(new sf::Sprite()) {

    canvas_sprite_->move(sf::Vector2f(300.f, 0.f));
    
    window_.reset(new lsWindow(800, 600));
    window_->set_listener(this); // app监听主窗口
    width_ = window_->width();
    height_ = window_->height();

    Config config;
    Platform::instance().set_config(config);
    Platform::instance().set_font_loader(GetPlatformFontLoader());
    Platform::instance().set_file_system(GetPlatformFileSystem("./assets/"));

    renderer_ = Renderer::Create();

    ViewConfig view_config;
    view_config.is_accelerated = false;
    view_ = renderer_->CreateView(width_, height_, view_config, nullptr);
    view_->LoadURL("file:///page.html");
    view_->set_view_listener(this);
    view_->set_load_listener(this);

    // // 用opencv加载图片
    // cv::Mat mat = cv::imread("../x64/Debug/supermarie.png");
    // cv::cvtColor(mat, rgbmat_, cv::COLOR_BGR2RGBA);

    camera_.release();
    camera_.open(0);
}

lsApp::~lsApp() {
    view_ = nullptr;
    renderer_ = nullptr;

    gui_buffer_ = nullptr;

    gui_texture_ = nullptr;
    gui_sprite_ = nullptr;
    canvas_texture_ = nullptr;
    canvas_sprite_ = nullptr;

    camera_.release();
}

void lsApp::event_loop(sf::RenderWindow &window, const sf::Event &event) {
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
    break;

    case sf::Event::Resized:
    {
        window_->OnResize(event.size.width, event.size.height);
    }
    break;

    default:
        break;
    }
}

void lsApp::Run() {
    sf::RenderWindow *wnd = window_->get_handle();

    std::chrono::milliseconds interval_ms(4);
    std::chrono::steady_clock::time_point next_paint = std::chrono::steady_clock::now();

    while (wnd->isOpen()) {
        long long timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            next_paint - std::chrono::steady_clock::now())
            .count();
        unsigned long timeout = timeout_ms <= 0 ? 0 : (unsigned long)timeout_ms;

        // 轮询事件
        sf::Event event;
        while (wnd->pollEvent(event)) {
            event_loop(*wnd, event);
        }

        // 更新应用逻辑
        renderer_->Update();

        // 时间间隔到则渲染
        timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            next_paint - std::chrono::steady_clock::now())
            .count();
        if (timeout_ms <= 0) {
            renderer_->RefreshDisplay(0);
            renderer_->Render();

            if (camera_.isOpened()) {
                camera_ >> rgbmat_;
                cv::cvtColor(rgbmat_, rgbmat_, cv::COLOR_BGR2RGBA);
            }

            Draw();

            next_paint = std::chrono::steady_clock::now() + interval_ms;
        }
    }
}

void lsApp::Draw() {
    Surface *surface = view_->surface();
    if (!surface->dirty_bounds().IsEmpty()) {
        // 绘制ultralight渲染的html
        RefPtr<Bitmap> bitmap = static_cast<BitmapSurface*>(surface)->bitmap();
        surface->ClearDirtyBounds();

        gui_buffer_ = bitmap->EncodePNG();// 最新版sdk才有这个接口
    }

    gui_texture_->loadFromMemory(gui_buffer_->data(), gui_buffer_->size());
    gui_sprite_->setTexture(*gui_texture_.get(), true);

    canvas_texture_->create(rgbmat_.cols, rgbmat_.rows);
    canvas_texture_->update(rgbmat_.data);
    canvas_sprite_->setTexture(*canvas_texture_.get(), true);

    window_->get_handle()->clear();

    window_->get_handle()->draw(*gui_sprite_.get());
    window_->get_handle()->draw(*canvas_sprite_.get());

    window_->PresentFrame();
}

void lsApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const String &url) {
}

void lsApp::OnResize(uint32_t width, uint32_t height) {
    // 更新ultralight的view
    view_->Resize(width, height);
    view_->set_needs_paint(true);

    // 更新sfml的view，使得sprite不拉伸
    sf::View sfview(sf::FloatRect(0, 0, (float)width, (float)height));
    window_->get_handle()->setView(sfview);
}
