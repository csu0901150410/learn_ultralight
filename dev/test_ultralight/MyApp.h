#include <Ultralight/Ultralight.h>

using namespace ultralight;

/**
 * 官方示例代码1 : 加载HTML文件并将其渲染到一个PNG文件
 *
 * 涉及的概念：
 * 1、指定平台相关的处理函数
 * 2、创建渲染器Renderer
 * 3、创建视图对象View
 * 4、加载本地文件到View
 * 5、自定义主循环等待文件加载完成
 * 6、渲染View
 * 7、保存渲染后的位图到PNG
 */

class MyApp : public LoadListener, public Logger {
private:
    RefPtr<Renderer> renderer_;
    RefPtr<View> view_;
    bool render_done_ = false;

public:
    MyApp();
    ~MyApp();

    void Run();

    // 复写虚函数
public:
    // Called when the page finishes loading a URL into a frame.
    virtual void OnFinishLoading(ultralight::View *caller, uint64_t frame_id,
                                 bool is_main_frame, const String &url) override;

    // Called when the library wants to display a log message.
    virtual void LogMessage(LogLevel log_level, const String &message) override;
};
