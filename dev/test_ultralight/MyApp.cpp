#include "MyApp.h"

#include <JavaScriptCore/JSRetainPtr.h>

#include <string>

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
    false, kWindowFlags_Titled | kWindowFlags_Resizable | kWindowFlags_Maximizable);

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

void MyApp::OnDOMReady(ultralight::View* caller,
                       uint64_t frame_id,
                       bool is_main_frame,
                       const String& url) {
  ///
  /// This is called when a frame's DOM has finished loading on the page.
  ///
  /// This is the best time to setup any JavaScript bindings.
  ///

  RefPtr<JSContext> context = caller->LockJSContext();
  SetJSContext(context->ctx());

  // 全局js对象，也就是window
  JSObject globalObj = JSGlobalObject();

  // c++和js交互，就两个需求
  // 1、在js中调用c++的函数（主要是这个），比如界面按钮按下，c++后台执行一些任务
  // 2、在c++中调用js函数，比如任务执行过程中，需要更新界面，比如进度条、调起子界面

  // 注册c++函数到js全局对象，只能注册成员函数，可以在成员函数内分发
  globalObj["expose_to_js"] = BindJSCallbackWithRetval(&MyApp::expose_to_js);

  // bug - 只能存在局部对象，存在成员call_dynamic_function_关闭窗口时会报错
  // 从js全局对象中获取js函数，同样，不同的js函数可以在这个函数中分发
  // call_dynamic_function_ = globalObj["call_to_js"];
  JSFunction jsFunc = globalObj["call_to_js"];

  // 在c++中调用js函数，指定函数名，指定参数
  // call_dynamic_function_({"dynamic_function_1", "js function called by c++"});
  jsFunc({"dynamic_function_1", "js function called by c++"});
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

JSValue command_00(const JSObject& thisObject, const JSArgs& args)
{
  // js调用c++

  // 作为演示，再调用一个js函数去更新界面
  JSObject globalObj = JSGlobalObject();
  JSFunction jsFunc = globalObj["call_to_js"];
  jsFunc({"dynamic_function_2", "c++ function called by js"});

  return JSValue();
}

JSValue MyApp::expose_to_js(const JSObject &thisObject, const JSArgs &args)
{
  ultralight::String name = args[0].ToString();

  std::string strname = std::string(name.utf8().data());
  if (0 == std::strcmp(strname.c_str(), "command_00"))
  {
    command_00(thisObject, JSArgs());
  }
  return JSValue();
}
