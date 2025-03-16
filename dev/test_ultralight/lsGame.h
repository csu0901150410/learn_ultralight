#pragma once

#include <SFML/Graphics.hpp>

#include "lsResourceHolder.hpp"

namespace lsResource {
    enum TextureID {
        kMarie,
    };

    enum FontID {
        kFiraCode,
    };
}

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
    sf::View m_view;

    lsResourceHolder<sf::Texture, lsResource::TextureID> m_textures;
    lsResourceHolder<sf::Font, lsResource::FontID> m_fonts;
    
    sf::Sprite m_sprite;
    sf::Text m_text;

    bool m_bMoveUp = false;
    bool m_bMoveDown = false;
    bool m_bMoveLeft = false;
    bool m_bMoveRight = false;

    float m_fMoveSpeed;// 移动速度
    sf::Time m_timePerFrame;// 每一帧的时间
    
    float m_fps;
};