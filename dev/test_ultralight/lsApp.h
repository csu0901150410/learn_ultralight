#pragma once

#include <AppCore/AppCore.h>

#include "lsWindow.h"

using namespace ultralight;

class lsApp : public WindowListener, public ViewListener, public LoadListener
{
public:
    lsApp();
    ~lsApp();

    void Run();

    void Draw();

    virtual void OnFinishLoading(ultralight::View* caller, uint64_t frame_id, bool is_main_frame,
        const String& url) override;

protected:
    std::unique_ptr<lsWindow> window_;// 主窗口
    int width_;
    int height_;

    RefPtr<Renderer> renderer_;
    RefPtr<View> view_;

    bool done_;
};
