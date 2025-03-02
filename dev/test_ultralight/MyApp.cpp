#include "MyApp.h"

#include <JavaScriptCore/JSRetainPtr.h>

#define WINDOW_WIDTH  600
#define WINDOW_HEIGHT 400

MyApp::MyApp() {
  ///
  /// Create our main App instance.
  ///
  app_ = App::Create();

  ///
  /// Create a resizable window by passing by OR'ing our window flags with
  /// kWindowFlags_Resizable.
  ///
  window_ = Window::Create(app_->main_monitor(), WINDOW_WIDTH, WINDOW_HEIGHT,
    false, kWindowFlags_Titled | kWindowFlags_Resizable);

  ///
  /// Create our HTML overlay-- we don't care about its initial size and
  /// position because it'll be calculated when we call OnResize() below.
  ///
  overlay_ = Overlay::Create(window_, 1, 1, 0, 0);

  ///
  /// Force a call to OnResize to perform size/layout of our overlay.
  ///
  OnResize(window_.get(), window_->width(), window_->height());

  ///
  /// Load a page into our overlay's View
  ///
  overlay_->view()->LoadURL("file:///app.html");

  ///
  /// Register our MyApp instance as an AppListener so we can handle the
  /// App's OnUpdate event below.
  ///
  app_->set_listener(this);

  ///
  /// Register our MyApp instance as a WindowListener so we can handle the
  /// Window's OnResize event below.
  ///
  window_->set_listener(this);

  ///
  /// Register our MyApp instance as a LoadListener so we can handle the
  /// View's OnFinishLoading and OnDOMReady events below.
  ///
  overlay_->view()->set_load_listener(this);

  ///
  /// Register our MyApp instance as a ViewListener so we can handle the
  /// View's OnChangeCursor and OnChangeTitle events below.
  ///
  overlay_->view()->set_view_listener(this);
}

MyApp::~MyApp() {
}

void MyApp::Run() {
  app_->Run();
}

void MyApp::OnUpdate() {
  ///
  /// This is called repeatedly from the application's update loop.
  ///
  /// You should update any app logic here.
  ///
}

void MyApp::OnClose(ultralight::Window* window) {
  app_->Quit();
}

void MyApp::OnResize(ultralight::Window* window, uint32_t width, uint32_t height) {
  ///
  /// This is called whenever the window changes size (values in pixels).
  ///
  /// We resize our overlay here to take up the entire window.
  ///
  overlay_->Resize(width, height);
}

void MyApp::OnFinishLoading(ultralight::View* caller,
                            uint64_t frame_id,
                            bool is_main_frame,
                            const String& url) {
  ///
  /// This is called when a frame finishes loading on the page.
  ///
}

// 注册到js的c++函数
JSValueRef expose_to_js(JSContextRef ctx, JSObjectRef function, JSObjectRef thisObject, size_t argumentCount, const JSValueRef arguments[], JSValueRef *exception)
{
  const char *str = "document.getElementById('result').innerText = 'c++ function called by js'";

  JSStringRef script = JSStringCreateWithUTF8CString(str);

  // 执行js代码
  JSEvaluateScript(ctx, script, NULL, NULL, 0, NULL);

  JSStringRelease(script);

  return JSValueMakeNull(ctx);
}

void MyApp::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {
  ///
  /// This is called when a frame's DOM has finished loading on the page.
  ///
  /// This is the best time to setup any JavaScript bindings.
  ///

  // 通过JSObjectCallAsFunction调用页面js函数
  auto scoped_context = caller->LockJSContext();
  JSContextRef ctx = (*scoped_context);

  JSRetainPtr<JSStringRef> str = adopt(JSStringCreateWithUTF8CString("call_to_js"));

  // 通过字符串获取js函数的值
  JSValueRef func = JSEvaluateScript(ctx, str.get(), NULL, NULL, 0, NULL);

  // 确保是对象
  if (JSValueIsObject(ctx, func))
  {
    // 转成对象
    JSObjectRef funcObj = JSValueToObject(ctx, func, NULL);

    // 确保是函数对象
    if (funcObj && JSObjectIsFunction(ctx, funcObj))
    {
      JSRetainPtr<JSStringRef> msg = adopt(JSStringCreateWithUTF8CString("js function called by c++"));

      // 构造参数
      const JSValueRef args[] = {
        JSValueMakeString(ctx, msg.get())
      };

      // 调用js函数
      size_t num_args = sizeof(args) / sizeof(JSValueRef);
      JSValueRef exception = 0;
      JSValueRef result = JSObjectCallAsFunction(ctx, funcObj, NULL, num_args, args, &exception);
    
      if (exception)
      {
        // 异常处理
      }

      if (result)
      {
        // 返回值处理
      }
    }
  }

  // 注册/暴露c++函数到js
  {
    // 根据c++函数创建js函数对象
    JSStringRef name = JSStringCreateWithUTF8CString("expose_to_js");
    JSObjectRef func = JSObjectMakeFunctionWithCallback(ctx, name, expose_to_js);
  
    // 全局js对象，也就是window
    JSObjectRef globalObj = JSContextGetGlobalObject(ctx);

    // 根据给定名称向js注册c++函数，js中可根据此名称调用c++函数
    JSObjectSetProperty(ctx, globalObj, name, func, kJSPropertyAttributeNone, NULL);

    JSStringRelease(name);
  }
}

void MyApp::OnChangeCursor(ultralight::View* caller,
                           Cursor cursor) {
  ///
  /// This is called whenever the page requests to change the cursor.
  ///
  /// We update the main window's cursor here.
  ///
  window_->SetCursor(cursor);
}

void MyApp::OnChangeTitle(ultralight::View* caller,
                          const String& title) {
  ///
  /// This is called whenever the page requests to change the title.
  ///
  /// We update the main window's title here.
  ///
  window_->SetTitle(title.utf8().data());
}
