// #include <Ultralight/Ultralight.h>
// #include <AppCore/Platform.h>

// using namespace ultralight;

// RefPtr<Renderer> g_renderer;
// RefPtr<View> g_view;

// void Init()
// {
//     Config config;
//     config.user_stylesheet = "body { background: purple; }";
//     Platform::instance().set_config(config);
// }

// void InitPlatform()
// {
//     Platform::instance().set_font_loader(GetPlatformFontLoader());
//     Platform::instance().set_file_system(GetPlatformFileSystem("."));
//     Platform::instance().set_logger(GetDefaultLogger("ultralight.log"));
// }

// void CreateRenderer()
// {
//     g_renderer = Renderer::Create();
// }

// void CreateView()
// {
//     ViewConfig config;
//     config.is_accelerated = false;// user cpu renderer
//     g_view = g_renderer->CreateView(800, 600, config, nullptr);
//     g_view->LoadHTML("<h1>Hello World!</h1>");
// }

// void CopyBitmapToTexture(RefPtr<Bitmap> bitmap)
// {
//     void *pixels = bitmap->LockPixels();
//     uint32_t width = bitmap->width();
//     uint32_t height = bitmap->height();
//     uint32_t stride = bitmap->row_bytes();

//     // copy pixels to texture

//     bitmap->UnlockPixels();
// }

// void UpdateLogic()
// {
//     g_renderer->Update();
// }

// void RenderOneFrame()
// {
//     g_renderer->Render();

//     // render all views
// }

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <iostream>

void event_loop(sf::RenderWindow& window, const sf::Event& event)
{
    switch (event.type)
    {
        case sf::Event::Resized:
            std::cout << "new width: " << event.size.width << std::endl;
            std::cout << "new height: " << event.size.height << std::endl;
        break;

        // 窗口初始状态是获得焦点状态的
        case sf::Event::LostFocus:
            std::cout << "lost focus" << std::endl;
        break;

        case sf::Event::GainedFocus:
            std::cout << "gained focus" << std::endl;
        break;

        case sf::Event::TextEntered:
            if (event.text.unicode < 128)
                std::cout << "text entered: " << static_cast<char>(event.text.unicode) << std::endl;
            else
                std::cout << "text entered: " << event.text.unicode << std::endl;
        break;

        // 键盘事件
        case sf::Event::KeyPressed:
            if (sf::Keyboard::Escape == event.key.code)
            {
                std::cout << "escape pressed" << std::endl;
                std::cout << "control: " << event.key.control << std::endl;
                std::cout << "alt: " << event.key.alt << std::endl;
                std::cout << "shift: " << event.key.shift << std::endl;
                std::cout << "system: " << event.key.system << std::endl;
            }
            else
            {
                std::cout << "key pressed: " << event.key.code << std::endl;
            }
        break;

        // 鼠标滚轮事件
        case sf::Event::MouseWheelScrolled:
            if (sf::Mouse::VerticalWheel == event.mouseWheelScroll.wheel)
                std::cout << "vertical wheel scroll: " << event.mouseWheelScroll.delta << std::endl;
            else if (sf::Mouse::HorizontalWheel == event.mouseWheelScroll.wheel)
                std::cout << "horizontal wheel scroll: " << event.mouseWheelScroll.delta << std::endl;
            else
                std::cout << "unknown wheel scroll" << std::endl;
            std::cout << "wheel movement: " << event.mouseWheelScroll.delta << std::endl;
            std::cout << "x: " << event.mouseWheelScroll.x << std::endl;
            std::cout << "y: " << event.mouseWheelScroll.y << std::endl;
        break;

        // 鼠标按键按下事件
        case sf::Event::MouseButtonPressed:
            if (sf::Mouse::Left == event.mouseButton.button)
            {
                std::cout << "left mouse button pressed" << std::endl;
                std::cout << "x: " << event.mouseButton.x << std::endl;
                std::cout << "y: " << event.mouseButton.y << std::endl;
            }
            else if (sf::Mouse::Right == event.mouseButton.button)
            {
                std::cout << "right mouse button pressed" << std::endl;
                std::cout << "x: " << event.mouseButton.x << std::endl;
                std::cout << "y: " << event.mouseButton.y << std::endl;
            }
            else if (sf::Mouse::Middle == event.mouseButton.button)
            {
                std::cout << "middle mouse button pressed" << std::endl;
                std::cout << "x: " << event.mouseButton.x << std::endl;
                std::cout << "y: " << event.mouseButton.y << std::endl;
            }
            else
            {
                std::cout << "unknown mouse button pressed" << std::endl;
                std::cout << "x: " << event.mouseButton.x << std::endl;
                std::cout << "y: " << event.mouseButton.y << std::endl;
            }
        break;

        // 鼠标移动事件
        case sf::Event::MouseMoved:
            // std::cout << "mouse moved" << std::endl;
            // std::cout << "x: " << event.mouseMove.x << std::endl;
            // std::cout << "y: " << event.mouseMove.y << std::endl;
        break;

        // 鼠标进入事件
        case sf::Event::MouseEntered:
            std::cout << "mouse entered" << std::endl;
        break;

        // 鼠标离开事件
        case sf::Event::MouseLeft:
            std::cout << "mouse left" << std::endl;
        break;

        case sf::Event::Closed:
            window.close();
        break;

        default:
        break;
    }
}

void sfml_skill()
{
    // 时间量，可以为负
    sf::Time t1 = sf::microseconds(1000000);// 微秒
    sf::Time t2 = sf::milliseconds(1000);// 毫秒
    sf::Time t3 = sf::seconds(1.0f);// 秒

    sf::Int64 usec = t3.asMicroseconds();
    sf::Int32 msec = t3.asMilliseconds();
    float     sec  = t3.asSeconds();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML demo");
    window.setVerticalSyncEnabled(true);// 开启垂直同步

    sf::CircleShape shape(100.0f);
    shape.setFillColor(sf::Color::Red);

    sf::Mouse::setPosition(sf::Vector2i(window.getSize()) / 2, window);

    // 纹理，其实就是图像
    sf::Texture texture;
    if (!texture.loadFromFile("../x64/Debug/result.png"))
    {
        return -1;
    }

    // 精灵，其实就是指定边界盒的图像，图像的一部分
    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setTextureRect(sf::IntRect(900, 500, 120, 80));
    sprite.move(sf::Vector2f(300, 300));

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            event_loop(window, event);
        }

        window.clear(sf::Color::Cyan);
        window.draw(shape);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
