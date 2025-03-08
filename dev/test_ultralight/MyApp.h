#pragma once

#include <AppCore/AppCore.h>

using namespace ultralight;

// 一个窗口两个overlay来显示html内容
// 通过监听窗口事件去调整overlay的大小

class MyApp : public WindowListener, public ViewListener
{
private:
    RefPtr<App> app_;
    RefPtr<Window> window_;
    RefPtr<Overlay> left_pane_;
    RefPtr<Overlay> right_pane_;

public:
    MyApp();
    ~MyApp();

    void Run();

    // 复写WindowListener的事件处理函数
public:
    virtual void OnClose(ultralight::Window* window) override;
    virtual void OnResize(ultralight::Window* window, uint32_t width_px, uint32_t height_px) override;

    // 复写ViewListener的事件处理函数
public:
    virtual void OnChangeCursor(View* caller, Cursor cursor) override;
};
