//
// Created by abdoulayedia on 20.12.2022
//
#include "SpriteManager.h"

namespace se {
	void SpriteManager::Render(sf::RenderTarget& target) const {
		target.draw(_sprite);
	}

	void SpriteManager::LoadSprite(const std::string& filepath) {
		_sprite.setTexture(AssetManager::Get().GetTexture(filepath));
	}

	void SpriteManager::LoadSprite(sf::Texture& texture) {
		_sprite.setTexture(texture);
	}
} // namespace se