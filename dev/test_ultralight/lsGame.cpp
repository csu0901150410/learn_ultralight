#include "lsGame.h"

#include <iostream>

lsGame::lsGame()
    : m_window(sf::VideoMode(800, 600), "demo")
    , m_texture()
    , m_sprite()
    , m_font()
    , m_text() {
    
    if (!m_texture.loadFromFile("../x64/Debug/supermarie.png")) {
        std::cout << "load texture failed" << std::endl;
    }

    if (!m_font.loadFromFile("../x64/Debug/FiraCode-Regular.ttf")) {
        std::cout << "load font failed" << std::endl;
    }

    m_text.setFont(m_font);
    m_text.setPosition(10.f, 10.f);

    m_sprite.setTexture(m_texture);
    m_sprite.setPosition(m_window.getSize().x / 2.f, m_window.getSize().y / 2.f);

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
