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

    // 用opencv加载图片
    cv::Mat mat = cv::imread("../x64/Debug/supermarie.png");
    cv::cvtColor(mat, rgbmat_, cv::COLOR_BGR2RGBA);
}

lsApp::~lsApp() {
    view_ = nullptr;
    renderer_ = nullptr;
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

            Draw();

            next_paint = std::chrono::steady_clock::now() + interval_ms;
        }
    }
}

void lsApp::Draw() {

    Surface *surface = view_->surface();
    if (surface->dirty_bounds().IsEmpty())
        return;

    // 绘制ultralight渲染的html
    RefPtr<Bitmap> bitmap = static_cast<BitmapSurface*>(surface)->bitmap();
    surface->ClearDirtyBounds();

    // static int si = 0;
    // if (0 == si++) {
    //     bitmap->WritePNG("../x64/Debug/result.png");// WritePNG也不能lock
    // } else {
    //     si = 2;
    // }

    // void *pixels = bitmap->LockPixels();// EncodePNG不能用这个lock

    RefPtr<Buffer> buffer = bitmap->EncodePNG();// 最新版sdk才有这个接口

    if (0)
    {
        bitmap->WritePNG("../x64/Debug/result.png");
    }

    uint32_t vw = view_->width();
    uint32_t vh = view_->height();
    uint32_t ww = window_->width();
    uint32_t wh = window_->height();


    // 创建纹理加载位图
    sf::Texture texture;
    texture.loadFromMemory(buffer->data(), buffer->size());
    // texture.loadFromMemory(pixels, bitmap->size());

    // texture.loadFromFile("../x64/Debug/result.png");

    // bitmap->UnlockPixels();

    // 创建精灵绑定纹理
    sf::Sprite sprite;
    sprite.setTexture(texture);

    sf::Vector2u windowSize = window_->get_handle()->getSize();
    sf::Vector2u bitmapSize = texture.getSize();
    float scaleX = static_cast<float>(windowSize.x) / bitmapSize.x;
    float scaleY = static_cast<float>(windowSize.y) / bitmapSize.y;
    sprite.setScale(scaleX, scaleY);

    window_->get_handle()->clear();

    // 绘制ultralight图像
    window_->get_handle()->draw(sprite);

    // 绘制opencv图像
    sf::Texture cvtexture;
    sf::Sprite cvsprite;

    // sf::Image image;
    // image.create(rgbmat_.cols, rgbmat_.rows, rgbmat_.ptr());
    // cvtexture.loadFromImage(image);

    cvtexture.create(rgbmat_.cols, rgbmat_.rows);
    cvtexture.update(rgbmat_.data);

    cvsprite.setTexture(cvtexture);
    cvsprite.move(sf::Vector2f(100.f, 100.f));
    window_->get_handle()->draw(cvsprite);

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
