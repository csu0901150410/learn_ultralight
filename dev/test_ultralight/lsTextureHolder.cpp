#include "lsTextureHolder.h"

#include <memory>
#include <cassert>

void lsTextureHolder::load(lsTextures::TextureID id, const std::string &filename) {
    std::unique_ptr<sf::Texture> texture(new sf::Texture());
    bool bsuccess = texture->loadFromFile(filename);
    if (!bsuccess) {
        throw std::runtime_error("TextureHolder::load - Failed to load " + filename);
    }

    auto ret = m_textureMap.insert(std::make_pair(id, std::move(texture)));
    assert(ret.second);// 必须始终为true，false就是重复插入了
}

sf::Texture &lsTextureHolder::get(lsTextures::TextureID id) {
    auto it = m_textureMap.find(id);
    assert(it != m_textureMap.end());
    return *(it->second);
}

const sf::Texture &lsTextureHolder::get(lsTextures::TextureID id) const {
    auto it = m_textureMap.find(id);
    assert(it != m_textureMap.end());
    return *(it->second);
}
