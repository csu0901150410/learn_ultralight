#include "lsGame.h"

lsGame::lsGame() :
    m_window(sf::VideoMode(800, 600), "demo"), m_shape() {
    m_shape.setRadius(40.0f);
    m_shape.setPosition(100.0f, 100.0f);
    m_shape.setFillColor(sf::Color::Red);
}

void lsGame::run() {
    while (m_window.isOpen()) {
        processEvents();
        update();
        render();

        // 延时
        sf::sleep(sf::milliseconds(100));
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

void lsGame::update() {
    sf::Vector2f movement(0.0f, 0.0f);
    if (m_bMoveUp)
        movement.y -= 1.0f;
    if (m_bMoveDown)
        movement.y += 1.0f;
    if (m_bMoveLeft)
        movement.x -= 1.0f;
    if (m_bMoveRight)
        movement.x += 1.0f;

    m_shape.move(movement * 5.0f);
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
