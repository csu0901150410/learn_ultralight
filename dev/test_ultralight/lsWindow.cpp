#include "lsWindow.h"

static int SFMLKeyCodeToUltralightKeyCode(int key);
static int SFMLModsToUltralightMods(const sf::Event& event);

lsWindow::lsWindow(uint32_t width, uint32_t height)
    : listener_(nullptr)
{
    window_ = new sf::RenderWindow(sf::VideoMode({ width, height }), "sfml window");
    window_->setVerticalSyncEnabled(true);// 开启垂直同步

    width_ = width;
    height_ = height;

    // SFML窗口事件传递到ultralight

    // 初始化各种光标实例
    sf::Cursor *pointerCursor = new sf::Cursor();
    pointerCursor->loadFromSystem(sf::Cursor::Type::Arrow);
    cursor_map_.insert(std::make_pair(ultralight::Cursor::kCursor_Pointer, pointerCursor));

    sf::Cursor *handCursor = new sf::Cursor();
    handCursor->loadFromSystem(sf::Cursor::Type::Hand);
    cursor_map_.insert(std::make_pair(ultralight::Cursor::kCursor_Hand, handCursor));
}

lsWindow::~lsWindow() {
    for (auto &item : cursor_map_)
        delete item.second;
}

uint32_t lsWindow::width() const {
    sf::Vector2u size = window_->getSize();
    return size.x;
}

uint32_t lsWindow::height() const {
    sf::Vector2u size = window_->getSize();
    return size.y;
}

void lsWindow::Close() {
    window_->close();
}

void lsWindow::SetCursor(ultralight::Cursor cursor) {
    // 根据传过来的ultralight光标类型设置对应的sfml光标
    if (cursor_map_.count(cursor)) {
        window_->setMouseCursor(*cursor_map_[cursor]);
    }
}

void lsWindow::PresentFrame()
{
    window_->display();
}

void lsWindow::OnResize(uint32_t width, uint32_t height) {
    listener_->OnResize(this, width, height);
}

void lsWindow::OnMouseEvent(const sf::Event &event) {
    // 把sfml的鼠标事件转换成ultralight事件传递给监听者
    if (sf::Event::MouseMoved != event.type 
        && sf::Event::MouseButtonPressed != event.type 
        && sf::Event::MouseButtonReleased != event.type)
        return;

    // 根据sfml鼠标事件结构体的数据填充ultralight的鼠标事件结构体
    ultralight::MouseEvent evt;

    // 鼠标移动、按下、抬起都可以取位置

    if (event.type == sf::Event::MouseMoved) {
        evt.type = ultralight::MouseEvent::kType_MouseMoved;
        evt.button = ultralight::MouseEvent::kButton_None;
        evt.x = event.mouseMove.x;
        evt.y = event.mouseMove.y;
    }
    else {
        // 鼠标左中右哪个按键按下
        switch (event.mouseButton.button) {
        case sf::Mouse::Left:
            evt.button = ultralight::MouseEvent::kButton_Left;
        break;

        case sf::Mouse::Right:
            evt.button = ultralight::MouseEvent::kButton_Right;
        break;

        case sf::Mouse::Middle:
            evt.button = ultralight::MouseEvent::kButton_Middle;
        break;

        default:
            evt.button = ultralight::MouseEvent::kButton_None;
        }

        // 鼠标按键是按下还是抬起
        if (event.type == sf::Event::MouseButtonPressed) {
            evt.type = ultralight::MouseEvent::kType_MouseDown;
        }
        else {
            evt.type = ultralight::MouseEvent::kType_MouseUp;
        }

        evt.x = event.mouseButton.x;
        evt.y = event.mouseButton.y;
    }

    // 包装好ultralight事件结构体后传递给监听者
    listener_->OnMouseEvent(this, evt);
}

void lsWindow::OnKeyEvent(const sf::Event &event) {

    // 要自己合成键盘事件，暂时还没有调通

    ultralight::KeyEvent evt;

    if (sf::Event::EventType::KeyPressed == event.type)
        evt.type = ultralight::KeyEvent::kType_RawKeyDown;
    else
        evt.type = ultralight::KeyEvent::kType_KeyUp;

    evt.virtual_key_code = SFMLKeyCodeToUltralightKeyCode(event.key.scancode);
    evt.native_key_code = event.key.scancode;
    evt.modifiers = SFMLModsToUltralightMods(event);

    ultralight::GetKeyIdentifierFromVirtualKeyCode(evt.virtual_key_code, evt.key_identifier);

    GetKeyFromVirtualKeyCode(evt.virtual_key_code, false, evt.text);
    evt.unmodified_text = evt.text;

    listener_->OnKeyEvent(this, evt);

    if (ultralight::KeyEvent::kType_RawKeyDown == evt.type)
    {
        // 单个字符，触发字符事件
        if (1 == evt.text.utf8().length())
        {
            evt.type = ultralight::KeyEvent::kType_Char;
            listener_->OnKeyEvent(this, evt);
        }
    }
}

int SFMLKeyCodeToUltralightKeyCode(int key)
{
    switch (key)
    {
    case sf::Keyboard::Scan::Num0: return ultralight::KeyCodes::GK_0;
    case sf::Keyboard::Scan::Num1: return ultralight::KeyCodes::GK_1;
    case sf::Keyboard::Scan::Num2: return ultralight::KeyCodes::GK_2;
    case sf::Keyboard::Scan::Num3: return ultralight::KeyCodes::GK_3;
    case sf::Keyboard::Scan::Num4: return ultralight::KeyCodes::GK_4;
    case sf::Keyboard::Scan::Num5: return ultralight::KeyCodes::GK_5;
    case sf::Keyboard::Scan::Num6: return ultralight::KeyCodes::GK_6;
    case sf::Keyboard::Scan::Num7: return ultralight::KeyCodes::GK_7;
    case sf::Keyboard::Scan::Num8: return ultralight::KeyCodes::GK_8;
    case sf::Keyboard::Scan::Num9: return ultralight::KeyCodes::GK_9;
    case sf::Keyboard::Scan::Numpad0: return ultralight::KeyCodes::GK_0;
    case sf::Keyboard::Scan::Numpad1: return ultralight::KeyCodes::GK_1;
    case sf::Keyboard::Scan::Numpad2: return ultralight::KeyCodes::GK_2;
    case sf::Keyboard::Scan::Numpad3: return ultralight::KeyCodes::GK_3;
    case sf::Keyboard::Scan::Numpad4: return ultralight::KeyCodes::GK_4;
    case sf::Keyboard::Scan::Numpad5: return ultralight::KeyCodes::GK_5;
    case sf::Keyboard::Scan::Numpad6: return ultralight::KeyCodes::GK_6;
    case sf::Keyboard::Scan::Numpad7: return ultralight::KeyCodes::GK_7;
    case sf::Keyboard::Scan::Numpad8: return ultralight::KeyCodes::GK_8;
    case sf::Keyboard::Scan::Numpad9: return ultralight::KeyCodes::GK_9;
    case sf::Keyboard::Scan::Period: return ultralight::KeyCodes::GK_DECIMAL;
    case sf::Keyboard::Scan::NumpadDecimal: return ultralight::KeyCodes::GK_DECIMAL;
    case sf::Keyboard::Scan::Backspace: return ultralight::KeyCodes::GK_BACK;
    default: return ultralight::KeyCodes::GK_UNKNOWN;
    }
}

int SFMLModsToUltralightMods(const sf::Event& event)
{
    int result = 0;
    if (event.key.shift)
        result |= ultralight::KeyEvent::kMod_ShiftKey;
    if (event.key.control)
        result |= ultralight::KeyEvent::kMod_CtrlKey;
    if (event.key.alt)
        result |= ultralight::KeyEvent::kMod_AltKey;
    if (event.key.system)
        result |= ultralight::KeyEvent::kMod_MetaKey;
    return result;

}
