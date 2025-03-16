#include "lsGame.h"

#include <iostream>

lsGame::lsGame()
    : m_window(sf::VideoMode(800, 600), "demo")
    , m_view(sf::View(sf::FloatRect(0.f, 0.f, 800.f, 600.f)))
    , m_textures()
    , m_sprite()
    , m_fonts()
    , m_text() {

    m_window.setView(m_view);
    
    m_textures.load(lsResource::TextureID::kMarie, "../x64/Debug/supermarie.png");
    m_fonts.load(lsResource::FontID::kFiraCode, "../x64/Debug/FiraCode-Regular.ttf");

    m_sprite.setTexture(m_textures.get(lsResource::TextureID::kMarie));
    m_sprite.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);

    m_text.setFont(m_fonts.get(lsResource::FontID::kFiraCode));
    m_text.setPosition(10.f, 10.f);

    m_timePerFrame = sf::seconds(1.f / 60.f);// 每秒60帧
    m_fMoveSpeed = 240.0f;
}

void lsGame::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    sf::Time timeoutFps = sf::Time::Zero;
    int frameCount = 0;
    while (m_window.isOpen()) {
        frameCount++;

        processEvents();

        // 叠加上个循环的耗时，看看此时间间隔要更新多少帧
        sf::Time deltaTime = clock.restart();
        timeSinceLastUpdate += deltaTime;
        timeoutFps += deltaTime;

        if (timeoutFps > sf::seconds(0.1f)) {
            // 每秒更新一次fps
            m_fps = frameCount / timeoutFps.asSeconds();
            frameCount = 0;
            timeoutFps = sf::Time::Zero;
        }

        while (timeSinceLastUpdate > m_timePerFrame)
        {
            timeSinceLastUpdate -= m_timePerFrame;
            processEvents();
            update(m_timePerFrame);
        }

        render();
    }
}

void lsGame::processEvents() {
    sf::Event event;
    while (m_window.pollEvent(event)) {
        switch (event.type) {
        case sf::Event::KeyPressed:
            handlePlayerInput(event.key.code, true);
        break;

        case sf::Event::KeyReleased:
            handlePlayerInput(event.key.code, false);
        break;

        case sf::Event::Resized:
        {
            // 计算精灵相对视图原点的位置
            float relx = m_sprite.getPosition().x / m_view.getSize().x;
            float rely = m_sprite.getPosition().y / m_view.getSize().y;

            // 更新视图的区域
            float rotation = m_view.getRotation();
            m_view.reset(sf::FloatRect(0, 0, (float)event.size.width, (float)event.size.height));
            m_view.setRotation(rotation);
            m_window.setView(m_view);

            // 更新精灵位置
            m_sprite.setPosition(m_view.getSize().x * relx, m_view.getSize().y * rely);
        }
        break;

        case sf::Event::Closed:
            m_window.close();
        break;
        }
    }
}

void lsGame::update(sf::Time deltaTime) {
    sf::Vector2f movement(0.0f, 0.0f);
    float speed = m_fMoveSpeed;
    if (m_bMoveUp)
        movement.y -= speed;
    if (m_bMoveDown)
        movement.y += speed;
    if (m_bMoveLeft)
        movement.x -= speed;
    if (m_bMoveRight)
        movement.x += speed;

    // 位移等于速度*时间
    movement *= deltaTime.asSeconds();
    m_sprite.move(movement);
}

void lsGame::render() {
    m_window.clear();

    m_window.draw(m_sprite);

    // fps转字符串时只保留整数部分
    sf::String fpsString = "FPS: " + std::to_string(static_cast<int>(m_fps));
    m_text.setString(fpsString);
    m_window.draw(m_text);

    m_window.display();
}

void lsGame::handlePlayerInput(sf::Keyboard::Key key, bool isPressed) {
    if (sf::Keyboard::W == key)
        m_bMoveUp = isPressed;
    else if (sf::Keyboard::S == key)
        m_bMoveDown = isPressed;
    else if (sf::Keyboard::A == key)
        m_bMoveLeft = isPressed;
    else if (sf::Keyboard::D == key)
        m_bMoveRight = isPressed;
}
