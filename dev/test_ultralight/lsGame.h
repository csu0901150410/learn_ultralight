#pragma once

#include <SFML/Graphics.hpp>

class lsGame {
public:
    lsGame();
    void run();

private:
    void processEvents();
    void update(sf::Time deltaTime);
    void render();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
    sf::RenderWindow m_window;
    sf::CircleShape m_shape;

    bool m_bMoveUp = false;
    bool m_bMoveDown = false;
    bool m_bMoveLeft = false;
    bool m_bMoveRight = false;

    float m_fMoveSpeed;// 移动速度
    sf::Time m_timePerFrame;// 每一帧的时间
};