//
// Created by abdoulayedia on 20.12.2022
//
#pragma once

#include "managers/AssetManager.h"

namespace se {
	// SpriteManager
	class SpriteManager {
	  public:
		SpriteManager() {}
		SpriteManager(const sf::Sprite& sprite): _sprite(sprite) {}
		~SpriteManager() {}

		sf::Sprite& GetSprite() { return _sprite; }

		void Render(sf::RenderTarget& target) const;
		void LoadSprite(const std::string& filepath);
		void LoadSprite(sf::Texture& texture);

		std::vector<sf::IntRect> SliceSprite(const std::string& filepath);

	  private:
		sf::Sprite               _sprite;
		std::vector<sf::IntRect> _frames;
	};
} // namespace se
