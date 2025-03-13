#include "lsGame.h"

lsGame::lsGame() :
    m_window(sf::VideoMode(800, 600), "demo"), m_shape() {
    m_shape.setRadius(40.0f);
    m_shape.setPosition(100.0f, 100.0f);
    m_shape.setFillColor(sf::Color::Red);

    m_timePerFrame = sf::seconds(1.f / 60.f);// 每秒60帧
    m_fMoveSpeed = 240.0f;
}

void lsGame::run() {
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    while (m_window.isOpen()) {
        processEvents();

        // 叠加上个循环的耗时，看看此时间间隔要更新多少帧
        timeSinceLastUpdate += clock.restart();
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
    m_shape.move(movement);
}

void lsGame::render() {
    m_window.clear();
    m_window.draw(m_shape);
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
