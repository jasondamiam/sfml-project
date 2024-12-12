#include "TextureManager.h"

std::unordered_map<std::string, sf::Texture> TextureManager::textures;

sf::Texture& TextureManager::getTexture(const std::string& fileName)
{
    if (textures.find(fileName) == textures.end()) {
        sf::Texture texture;
        if (!texture.loadFromFile(fileName)) {
            throw std::runtime_error("No texture found idk error error error");
        }
        textures[fileName] = std::move(texture);
    }
    return textures[fileName];
}
