#include "MyApp.h"

#define WINDOW_WIDTH    900
#define WINDOW_HEIGHT   600
#define LEFT_PANE_WIDTH 200

MyApp::MyApp()
{
    app_ = App::Create();
    window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT, false, kWindowFlags_Titled | kWindowFlags_Resizable);

    left_pane_ = Overlay::Create(window_, 100, 100, 0, 0);
    right_pane_ = Overlay::Create(window_, 100, 100, 0, 0);
    OnResize(window_.get(), window_->width(), window_->height());

    // 两个overlay各自加载html
    left_pane_->view()->LoadURL("file:///sidebar.html");
    right_pane_->view()->LoadURL("file:///content.html");

    // 当前类接管窗口和两个overlay的事件
    window_->set_listener(this);
    left_pane_->view()->set_view_listener(this);
    right_pane_->view()->set_view_listener(this);
}

MyApp::~MyApp()
{
}

void MyApp::Run()
{
    app_->Run();
}

void MyApp::OnClose(ultralight::Window* window)
{
    app_->Quit();
}

void MyApp::OnResize(ultralight::Window* window, uint32_t width_px, uint32_t height_px)
{
    // 左边的overlay定长，再计算右边overlay的剩余宽度
    uint32_t left_pane_width_px = window_->ScreenToPixels(LEFT_PANE_WIDTH);
    left_pane_->Resize(left_pane_width_px, height_px);

    int right_pane_width_px = (int)width_px - left_pane_width_px;// 可能为负
    if (right_pane_width_px <= 1)
        right_pane_width_px = 1;
    right_pane_->Resize(right_pane_width_px, height_px);

    // 设置overlay的原点
    left_pane_->MoveTo(0, 0);
    right_pane_->MoveTo(left_pane_width_px, 0);
}

// 鼠标进出视图的时候，样式会改变，将其设置为窗口的鼠标样式
void MyApp::OnChangeCursor(View *caller, Cursor cursor) {
    // window_->SetCursor(cursor);
    window_->SetCursor(kCursor_Hand); // 强制指定手形光标
}
