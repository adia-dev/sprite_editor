//
// Created by abdoulayedia on 20.12.2022
//
#pragma once

#include "managers/AssetManager.h"

#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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

		std::vector<sf::IntRect>
		SliceSprite(const sf::IntRect& roiRect = {0, 0, 0, 0});

		static std::vector<sf::IntRect> SliceSprite(const sf::Texture& texture,
		                                            const sf::IntRect& roiRect);
		static std::vector<sf::IntRect>
		SliceSprite(const std::string& filepath);
		static std::vector<sf::IntRect> SliceSprite(const std::string& filepath,
		                                            const sf::IntRect& roiRect);

		static cv::Mat TextureToOpenCVMat(const sf::Texture& texture);

		static void removeTextureBackground(
		    sf::Texture&     texture,
		    uint16_t         x,
		    u_int16_t        y,
		    const sf::Color& newColor  = sf::Color::Transparent,
		    double           threshold = 1.0);

		sf::Texture removeTextureBackground(
		    uint16_t         x,
		    u_int16_t        y,
		    const sf::Color& newColor  = sf::Color::Transparent,
		    double           threshold = 1.0);

		sf::Texture removeTextureBackground(
		    const sf::Vector2i& pixelPosition,
		    const sf::Color&    newColor  = sf::Color::Transparent,
		    double              threshold = 1.0);

	  private:
		sf::Sprite               _sprite;
		std::vector<sf::IntRect> _frames;
	};
} // namespace se
