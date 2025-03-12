#pragma once

#include <SFML/Graphics.hpp>

class lsGame {
public:
    lsGame();
    void run();

private:
    void processEvents();
    void update();
    void render();

    void handlePlayerInput(sf::Keyboard::Key key, bool isPressed);

private:
    sf::RenderWindow m_window;
    sf::CircleShape m_shape;

    bool m_bMoveUp;
    bool m_bMoveDown;
    bool m_bMoveLeft;
    bool m_bMoveRight;
};