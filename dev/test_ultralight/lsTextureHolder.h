#pragma once

#include <map>
#include <memory>

#include <SFML/Graphics.hpp>

namespace lsTextures {
    enum TextureID {
        Player,
    };
}

class lsTextureHolder {
public:
    lsTextureHolder() {}

public:
    void load(lsTextures::TextureID id, const std::string& filename);

    sf::Texture& get(lsTextures::TextureID id);

    const sf::Texture& get(lsTextures::TextureID id) const;

private:
    std::map<lsTextures::TextureID, std::unique_ptr<sf::Texture>> m_textureMap;
};
