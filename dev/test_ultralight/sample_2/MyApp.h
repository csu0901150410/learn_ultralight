#pragma once

#include <AppCore/AppCore.h>

using namespace ultralight;

class MyApp : public WindowListener, public ViewListener, public LoadListener
{
private:
    RefPtr<App> app_;// 管理主窗口，提供消息循环、渲染器
    RefPtr<Window> window_;// 主窗口
    RefPtr<Overlay> overlay_;// 在主窗口内显示html页面

public:
    MyApp();
    ~MyApp();
    void Run();

    // 复写WindowListener的事件处理函数
public:
    virtual void OnClose(Window* window) override;
    virtual void OnResize(Window* window, uint32_t width, uint32_t height) override;

    // 复写ViewListener的事件处理函数
public:
    virtual void OnChangeCursor(View* caller, Cursor cursor) override;

    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame, const String& url) override;
};
