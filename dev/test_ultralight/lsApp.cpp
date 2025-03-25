#include "lsApp.h"

#include <chrono>

#include <JavaScriptCore/JSRetainPtr.h>

lsApp::lsApp() 
    : gui_texture_(new sf::Texture())
    , canvas_texture_(new sf::Texture()) 
    , gui_sprite_(new sf::Sprite())
    , canvas_sprite_(new sf::Sprite())
    , sub_gui_texture_(new sf::Texture())
    , sub_gui_sprite_(new sf::Sprite()) {

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

    time_per_frame_ = sf::seconds(1.0f / 60.0f);
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

void lsApp::create_subwindow() {
    // 创建子窗口
    if (!m_subwindow) {
        m_subwindow = std::make_unique<lsWindow>(400, 300); // 设置合适的窗口大小
        m_subwindow->set_listener(this);
        m_subwindow->get_handle()->setTitle("sub window");

        // 为子窗口创建新的View
        ViewConfig view_config;
        view_config.is_accelerated = false;
        m_subview = renderer_->CreateView(400, 300, view_config, nullptr);
        
        // 加载设置页面
        m_subview->LoadURL("file:///page2.html");
        m_subview->set_view_listener(this);
        m_subview->set_load_listener(this);
    }
}

void lsApp::run() {
    sf::RenderWindow *wnd = window_->get_handle();

    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timeoutFps = sf::Time::Zero;

    while (wnd->isOpen()) {
        processEvents();

        // 叠加上个循环的耗时，看看此时间间隔要更新多少帧
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;

        while (timeSinceLastUpdate > time_per_frame_) {
            timeSinceLastUpdate -= time_per_frame_;
            processEvents();
            update(time_per_frame_);
        }

        render();
    }
}

void lsApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const String &url) {
    if (is_main_frame) {
        // 这里打断点，先执行render才到这里加载完毕，会导致渲染时gui位图还没准备好
        // 加载完成也不等于渲染完成，所以render还是要判断gui_buffer_
        int a = 100;
    }
}

void lsApp::OnDOMReady(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const String &url) {
    RefPtr<JSContext> context = caller->LockJSContext();
    SetJSContext(context->ctx());

    // 全局js对象，也就是window
    JSObject globalObj = JSGlobalObject();

    // c++和js交互，就两个需求
    // 1、在js中调用c++的函数（主要是这个），比如界面按钮按下，c++后台执行一些任务
    // 2、在c++中调用js函数，比如任务执行过程中，需要更新界面，比如进度条、调起子界面

    // 注册c++函数到js全局对象，只能注册成员函数，可以在成员函数内分发
    globalObj["expose_to_js"] = BindJSCallbackWithRetval(&lsApp::expose_to_js);
}

void lsApp::OnChangeCursor(ultralight::View *caller, Cursor cursor) {
    window_->SetCursor(cursor);
}

void lsApp::OnResize(uint32_t width, uint32_t height) {
    // 更新ultralight的view
    view_->Resize(width, height);
    view_->set_needs_paint(true);

    // 更新sfml的view，使得sprite不拉伸
    sf::View sfview(sf::FloatRect(0, 0, (float)width, (float)height));
    window_->get_handle()->setView(sfview);
}

void lsApp::OnMouseEvent(const ultralight::MouseEvent &event) {
    // sfml窗口过来的事件传递给ultralight的view
    view_->FireMouseEvent(event);
}

JSValue command_00(const JSObject& thisObject, const JSArgs& args)
{
    // js调用c++
    return JSValue();
}

JSValue lsApp::expose_to_js(const JSObject &thisObject, const JSArgs &args) {
    ultralight::String name = args[0].ToString();
    std::string strname = std::string(name.utf8().data());

    if (strname == "command_00") {
        create_subwindow();
    }
    return JSValue();
}

void lsApp::processEvents() {
    // 处理主窗口事件
    sf::Event event;
    while (window_->get_handle()->pollEvent(event)) {
        switch (event.type) {
        case sf::Event::EventType::Closed:
        {
            window_->get_handle()->close();
        }
        break;

        case sf::Event::EventType::Resized:
        {
            window_->OnResize(event.size.width, event.size.height);
        }
        break;

        case sf::Event::EventType::MouseMoved:
        case sf::Event::EventType::MouseButtonPressed:
        case sf::Event::EventType::MouseButtonReleased:
        {
            // app轮询事件，转发给window，window打包成ultralight事件后，
            // 再将事件转发给其监听者
            window_->OnMouseEvent(event);
        }
        break;
        }
    }

    // 处理子窗口事件（如果存在）
    if (m_subwindow && m_subwindow->get_handle()->isOpen()) {
        sf::Event sub_event;
        while (m_subwindow->get_handle()->pollEvent(sub_event)) {
            if (sub_event.type == sf::Event::Closed) {
                m_subwindow->get_handle()->close();
            }
            else if (sub_event.type == sf::Event::Resized) {
                m_subwindow->OnResize(sub_event.size.width, sub_event.size.height);
                if (m_subview) {
                    m_subview->Resize(sub_event.size.width, sub_event.size.height);
                }
            }
            else if (sub_event.type == sf::Event::MouseMoved || 
                     sub_event.type == sf::Event::MouseButtonPressed || 
                     sub_event.type == sf::Event::MouseButtonReleased) {
                    m_subwindow->OnMouseEvent(event);
            }
        }
    }
}

void lsApp::update(sf::Time deltaTime) {
    // 应用更新逻辑

    // ultralight更新
    renderer_->Update();
}

void lsApp::render() {
    // ultralight渲染
    renderer_->RefreshDisplay(0);
    renderer_->Render();

    Surface *surface = view_->surface();
    if (!surface->dirty_bounds().IsEmpty()) {
        // 绘制ultralight渲染的html
        RefPtr<Bitmap> bitmap = static_cast<BitmapSurface*>(surface)->bitmap();
        surface->ClearDirtyBounds();

        gui_buffer_ = bitmap->EncodePNG();// 最新版sdk才有这个接口
    }

    if (camera_.isOpened()) {
        camera_ >> rgbmat_;
        cv::cvtColor(rgbmat_, rgbmat_, cv::COLOR_BGR2RGBA);
    }

    if (gui_buffer_.get() == nullptr) {
        return;// 要等ui渲染完毕
    }

    if (rgbmat_.empty()) {
        return;
    }

    gui_texture_->loadFromMemory(gui_buffer_->data(), gui_buffer_->size());
    gui_sprite_->setTexture(*gui_texture_.get(), true);

    canvas_texture_->create(rgbmat_.cols, rgbmat_.rows);
    canvas_texture_->update(rgbmat_.data);
    canvas_sprite_->setTexture(*canvas_texture_.get(), true);

    window_->get_handle()->clear();
    window_->get_handle()->draw(*gui_sprite_.get());
    window_->get_handle()->draw(*canvas_sprite_.get());

    // 对采集到的帧进行处理
    processCvFrame();

    window_->get_handle()->display();


    // 渲染子窗口（如果存在）
    if (m_subwindow && m_subwindow->get_handle()->isOpen() && m_subview) {
        m_subwindow->get_handle()->clear();
        
        Surface* sub_surface = m_subview->surface();
        if (sub_surface && !sub_surface->dirty_bounds().IsEmpty()) {
            // 绘制ultralight渲染的html
            RefPtr<Bitmap> bitmap = static_cast<BitmapSurface*>(sub_surface)->bitmap();
            sub_surface->ClearDirtyBounds();
    
            sub_gui_buffer_ = bitmap->EncodePNG();// 最新版sdk才有这个接口
        }

        sub_gui_texture_->loadFromMemory(sub_gui_buffer_->data(), sub_gui_buffer_->size());
        sub_gui_sprite_->setTexture(*sub_gui_texture_.get(), true);

        m_subwindow->get_handle()->draw(*sub_gui_sprite_.get());
        
        m_subwindow->get_handle()->display();
    }
}

void lsApp::processCvFrame() {
    // 来点灰度，sfml需要四通道，最后要转回
    cv::Mat grayFrame = rgbmat_.clone();
    cv::cvtColor(grayFrame, grayFrame, cv::COLOR_RGBA2GRAY);
    cv::cvtColor(grayFrame, grayFrame, cv::COLOR_GRAY2BGRA);

    sf::Texture texture;
    sf::Sprite sprite;
    texture.create(grayFrame.cols, grayFrame.rows);
    texture.update(grayFrame.data);
    sprite.setTexture(texture);
    sprite.move(sf::Vector2f(300.f, 500.f));

    window_->get_handle()->draw(sprite);
}
