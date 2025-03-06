#include "MyApp.h"

#include <AppCore/AppCore.h>

#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>// for put_time

#include <memory>
#include <thread>

MyApp::MyApp() {
    Config config;
    Platform::instance().set_config(config);

    // 未使用App::Create()时，需要指定平台相关的处理程序，必须指定的是
    // font_loader和file_system，这里使用appcore获取到的，也可以指定
    // 自己实现的处理程序
    Platform::instance().set_font_loader(GetPlatformFontLoader());

    // appcore提供的文件系统处理程序，可以指定资源路径（用 file:/// 访问）
    Platform::instance().set_file_system(GetPlatformFileSystem("./assets/"));

    // MyApp继承自Logger，可以将this设置为logger
    Platform::instance().set_logger(this);

    // 创建渲染器，整个app只能创建一次，并且要在所有View对象之前Platform设置之后创建
    renderer_ = Renderer::Create();

    ViewConfig view_config;// 使用默认配置就行
    view_ = renderer_->CreateView(1920, 1080, view_config, nullptr);

    // 将app设置为view的加载监听者，这样view的加载事件可以交由app处理
    view_->set_load_listener(this);

    // 加载url指定的文件到View
    view_->LoadURL("file:///page.html");

    // 注意，View将会响应html文件的加载事件，并将其离屏渲染到surface
    // 这里仅是将渲染的位图保存为PNG，不需要窗口Window
}

MyApp::~MyApp() {
    // 置空就好，智能指针管理的内存会自动释放
    view_ = nullptr;
    renderer_ = nullptr;
}

void MyApp::Run() {
    LogMessage(LogLevel::Info, "Starting Run(), waiting for page to load...");

    // 等待加载完成
    do {
        renderer_->Update();
        std::this_thread::sleep_for(std::chrono::milliseconds(10));// 10ms
    } while(!render_done_);

    // 渲染View
    renderer_->Render();

    // 没有通过Platform::set_surface_factory设置自己的surface的话，默认渲染到BitmapSurface
    BitmapSurface *surface = (BitmapSurface*)view_->surface();
    RefPtr<Bitmap> bitmap = surface->bitmap();
    bitmap->WritePNG("../x64/Debug/result.png");

    LogMessage(LogLevel::Info, "Saved a render of our page to result.png.");
}

void MyApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id,
    bool is_main_frame, const String &url) {
    if (is_main_frame)
    {
        LogMessage(LogLevel::Info, "Our page has loaded!");
        render_done_ = true;
    }
}

void MyApp::LogMessage(LogLevel log_level, const String &message) {
    // 自定义日志信息输出

    // 获取当前时间并格式化为字符串
    auto getCurrentTime = [](){
        // 获取当前时间点
        auto now = std::chrono::system_clock::now();
        // 转换为 time_t 类型
        std::time_t now_time_t = std::chrono::system_clock::to_time_t(now);
        // 转换为 tm 结构体
        std::tm now_tm = *std::localtime(&now_time_t);

        // 使用 stringstream 格式化时间
        std::stringstream ss;
        ss << std::put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    };

    std::cout << getCurrentTime() << " < " << message.utf8().data() << std::endl;
}
