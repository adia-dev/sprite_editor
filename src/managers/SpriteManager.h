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
		~SpriteManager() {}

		sf::Sprite& GetSprite() { return _sprite; }

		void Render(sf::RenderTarget& target) const;
		void LoadSprite(const std::string& filepath);
		void LoadSprite(sf::Texture& texture);

	  private:
		sf::Sprite _sprite;
	};
} // namespace se
