//
// Created by abdoulayedia on 20.12.2022
//
#pragma once

#include "managers/AssetManager.h"

#include <functional>
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

namespace se {

	class SpriteManager {
	  public:
		SpriteManager();

		SpriteManager(const sf::Sprite &sprite);

		~SpriteManager();

		sf::Sprite &GetSprite();

		void Render(sf::RenderTarget &target);

		void LoadSprite(const std::string &filepath);
		void LoadSprite(sf::Texture &texture);

		std::vector<sf::IntRect>        SliceSprite(const sf::IntRect &roiRect = {0, 0, 0, 0});
		static std::vector<sf::IntRect> SliceSprite(const sf::Texture &texture, const sf::IntRect &roiRect);
		static std::vector<sf::IntRect> SliceSprite(const std::string &filepath);
		static std::vector<sf::IntRect> SliceSprite(const std::string &filepath, const sf::IntRect &roiRect);

		static cv::Mat TextureToOpenCVMat(const sf::Texture &texture);

		static void removeTextureBackground(sf::Texture     &texture,
		                                    uint16_t         x,
		                                    u_int16_t        y,
		                                    const sf::Color &newColor  = sf::Color::Transparent,
		                                    double           threshold = 1.0);

		sf::Texture removeTextureBackground(uint16_t         x,
		                                    u_int16_t        y,
		                                    const sf::Color &newColor  = sf::Color::Transparent,
		                                    double           threshold = 1.0);

		sf::Texture removeTextureBackground(const sf::Vector2i &pixelPosition,
		                                    const sf::Color    &newColor  = sf::Color::Transparent,
		                                    double              threshold = 1.0);

		void                      SetFrames(const std::vector<sf::IntRect> &frames);
		std::vector<sf::IntRect> &GetFrames();

		void SetCurrentFrameIndex(int index);
		void SwapFrames(int index1, int index2);
		int  GetCurrentFrameIndex() const;

		sf::IntRect GetCurrentFrame() const;
		void        SetCurrentFrame(const sf::IntRect &rect);

		void DeleteFrame(int index);
		void AddFrame(const sf::IntRect &frame, int index = -1);
		void AddFrames(const std::vector<sf::IntRect> &frames, int index = -1);

		void  SetAnimationTimer(float t);
		float GetAnimationTimer() const;

		void UpdateAnimation(float dt);
		void RenderAnimation(sf::RenderTarget &target);

		char AnimationName[256] = {"Idle"};

	  private:
		sf::Sprite _sprite;          // The sprite managed by the SpriteManager.
		sf::Sprite _animationSprite; // The sprite used for rendering the animation.

		std::vector<sf::IntRect> _frames;      // The individual frames of the sliced sprite.
		std::vector<sf::IntRect> _worldFrames; // The individual frames rectangles, transformed to world coordinates.

		sf::IntRect _currentFrame;            // The current frame of the animation.
		int         _currentFrameIndex = 0;   // The index of the current frame of the animation.
		float       _animationTimer    = 0.f; // The timer used for animating the sprite.
	};

} // namespace se
