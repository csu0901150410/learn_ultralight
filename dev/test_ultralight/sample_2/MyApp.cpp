#include "MyApp.h"

#include <Ultralight/Ultralight.h>

MyApp::MyApp() {
    Platform::instance().set_font_loader(GetPlatformFontLoader());
    Platform::instance().set_file_system(GetPlatformFileSystem("./assets/"));

    app_ = App::Create();

    // 创建主窗口
    window_ = Window::Create(app_->main_monitor(), 800, 600, false, kWindowFlags_Titled | kWindowFlags_Resizable | kWindowFlags_Maximizable);
    window_->SetTitle("Basic App");
    window_->MoveToCenter();

    // 创建overlay覆盖整个主窗口
    overlay_ = Overlay::Create(window_, window_->width(), window_->height(), 0, 0);
    overlay_->view()->LoadURL("file:///page.html");

    // 设置监听者为app，这样app可以处理窗口和视图的事件
    window_->set_listener(this);
    overlay_->view()->set_view_listener(this);
}

MyApp::~MyApp() {
}

void MyApp::OnClose(Window *window) {
    app_->Quit();
}

void MyApp::OnResize(Window *window, uint32_t width, uint32_t height) {
    overlay_->Resize(width, height); // overlay跟着变化，否则会显示黑色背景
}

// 鼠标进出视图的时候，样式会改变，将其设置为窗口的鼠标样式
void MyApp::OnChangeCursor(View *caller, Cursor cursor) {
    // window_->SetCursor(cursor);
    window_->SetCursor(kCursor_Hand); // 强制指定手形光标
}

void MyApp::OnFinishLoading(ultralight::View *caller, uint64_t frame_id, bool is_main_frame, const String &url) {
    
}

void MyApp::Run() {
    // 由appcore内部管理消息循环，renderer/view等都是appcore创建的
    // 如果只是在html上边玩，这就够了，如果想要在窗口上绘制ultralight
    // 渲染的html之外的东西，就要自己创建renderer和管理消息循环。
    app_->Run();
}
