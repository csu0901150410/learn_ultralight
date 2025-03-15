#pragma once

#include <SFML/Graphics.hpp>

#include "lsTextureHolder.h"

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

    lsTextureHolder m_texture;// 管理所有纹理
    sf::Sprite m_sprite;

    bool m_bMoveUp = false;
    bool m_bMoveDown = false;
    bool m_bMoveLeft = false;
    bool m_bMoveRight = false;

    float m_fMoveSpeed;// 移动速度
    sf::Time m_timePerFrame;// 每一帧的时间

    sf::Font m_font;
    sf::Text m_text;
    float m_fps;
};