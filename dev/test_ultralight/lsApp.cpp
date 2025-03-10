#include "lsApp.h"

#include <chrono>

lsApp::lsApp() {
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

    done_ = false;
}

lsApp::~lsApp() {
    view_ = nullptr;
    renderer_ = nullptr;
}

void event_loop(sf::RenderWindow &window, const sf::Event &event) {
    switch (event.type) {
    case sf::Event::Closed:
        window.close();
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
        sf::Event event;
        while (wnd->pollEvent(event)) {
            long long timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                                       next_paint - std::chrono::steady_clock::now())
                                       .count();
            unsigned long timeout = timeout_ms <= 0 ? 0 : (unsigned long)timeout_ms;

            if (!(timeout > 0))
                event_loop(*wnd, event);

            timeout_ms = std::chrono::duration_cast<std::chrono::milliseconds>(
                             next_paint - std::chrono::steady_clock::now())
                             .count();

            renderer_->Update();

            if (timeout_ms <= 0) {
                renderer_->Render();

                Draw();

                next_paint = std::chrono::steady_clock::now() + interval_ms;
            }
        }
    }
}

void lsApp::Draw() {
    if (!done_)
        return;

    // 绘制ultralight渲染的html
    BitmapSurface *bitmap_surface = (BitmapSurface *)view_->surface();
    RefPtr<Bitmap> bitmap = bitmap_surface->bitmap();

    static int si = 0;
    if (0 == si++) {
        bitmap->WritePNG("../x64/Debug/result.png");// WritePNG也不能lock
    } else {
        si = 2;
    }

    // void *pixels = bitmap->LockPixels();// EncodePNG不能用这个lock

    RefPtr<Buffer> buffer = bitmap->EncodePNG();// 最新版sdk才有这个接口


    // 创建纹理加载位图
    sf::Texture texture;
    texture.loadFromMemory(buffer->data(), buffer->size());
    // texture.loadFromMemory(pixels, bitmap->size());

    // texture.loadFromFile("../x64/Debug/result.png");

    // bitmap->UnlockPixels();

    // 创建精灵绑定纹理
    sf::Sprite sprite;
    sprite.setTexture(texture);

    // 绘制精灵
    window_->get_handle()->draw(sprite);

    window_->PresentFrame();
}

void lsApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const String &url) {
    if (is_main_frame)
        done_ = true;
}
