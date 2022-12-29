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
	/**
	 * @class SpriteManager
	 *
	 * @brief Manages the loading and slicing of sprites, as well as providing
	 * functions to remove the background of a sprite's texture.
	 *
	 * The SpriteManager class provides functions for loading and slicing
	 * sprites, as well as removing the background of a sprite's texture. It can
	 * load sprites from file or from an existing sf::Texture, and provides
	 * functions to slice the sprite into individual frames. The SpriteManager
	 * also provides functions to remove the background of a sprite's texture
	 * using OpenCV.
	 */
	class SpriteManager {
	  public:
		/**
		 * @brief Constructs a new SpriteManager object with an empty sprite.
		 */
		SpriteManager();

		/**
		 * @brief Constructs a new SpriteManager object with the given sprite.
		 *
		 * @param sprite The sprite to use for the SpriteManager.
		 */
		SpriteManager(const sf::Sprite &sprite);

		/**
		 * @brief Destroys the SpriteManager object.
		 */
		~SpriteManager();

		/**
		 * @brief Returns a reference to the sprite managed by the
		 * SpriteManager.
		 *
		 * @return A reference to the sprite managed by the SpriteManager.
		 */
		sf::Sprite &GetSprite();

		/**
		 * @brief Renders the sprite managed by the SpriteManager to the given
		 * render target.
		 *
		 * @param target The render target to render the sprite to.
		 */
		void Render(sf::RenderTarget &target) const;

		/**
		 * @brief Loads a sprite from file and sets it as the sprite managed by
		 * the SpriteManager.
		 *
		 * @param filepath The filepath of the sprite image to load.
		 */
		void LoadSprite(const std::string &filepath);

		/**
		 * @brief Sets the given texture as the sprite managed by the
		 * SpriteManager.
		 *
		 * @param texture The texture to use for the sprite managed by the
		 * SpriteManager.
		 */
		void LoadSprite(sf::Texture &texture);

		/**
		 * @brief Slices the sprite managed by the SpriteManager into individual
		 * frames.
		 *
		 * @param roiRect The region of interest (ROI) rect to use for slicing
		 * the sprite. If the ROI rect is not specified, the entire sprite will
		 * be used for slicing.
		 *
		 * @return A vector of sf::IntRect objects representing the individual
		 * frames of the sliced sprite.
		 */
		std::vector<sf::IntRect> SliceSprite(const sf::IntRect &roiRect = {0, 0, 0, 0});

		/**
		 * @brief Slices the given texture into individual frames.
		 *
		 * @param texture The texture to slice.
		 * @param roiRect The region of interest (ROI) rect to use for slicing
		 * the texture. If the ROI rect is not specified, the entire texture
		 * will be used for slicing.
		 *
		 * @return A vector of sf::IntRect objects representing the individual
		 * frames of the sliced texture.
		 */
		static std::vector<sf::IntRect> SliceSprite(const sf::Texture &texture, const sf::IntRect &roiRect);

		/**
		 * @brief Slices the sprite at the given filepath into individual
		 * frames.
		 *
		 * @param filepath The filepath of the sprite image to slice.
		 *
		 * @return A vector of sf::IntRect objects representing the individual
		 * frames of the sliced sprite.
		 */
		static std::vector<sf::IntRect> SliceSprite(const std::string &filepath);

		/**
		 * @brief Slices the sprite at the given filepath into individual
		 * frames.
		 *
		 * @param filepath The filepath of the sprite image to slice.
		 * @param roiRect The region of interest (ROI) rect to use for slicing
		 * the sprite. If the ROI rect is not specified, the entire sprite will
		 * be used for slicing.
		 *
		 * @return A vector of sf::IntRect objects representing the individual
		 * frames of the sliced sprite.
		 */
		static std::vector<sf::IntRect> SliceSprite(const std::string &filepath, const sf::IntRect &roiRect);

		/**
		 * @brief Converts the given sf::Texture to an OpenCV Mat object.
		 *
		 * @param texture The texture to convert.
		 *
		 * @return An OpenCV Mat object representing the given texture.
		 */
		static cv::Mat TextureToOpenCVMat(const sf::Texture &texture);

		/**
		 * @brief Removes the background of the given texture and sets the new
		 * background color to the specified color.
		 *
		 * @param texture The texture to remove the background from.
		 * @param x The x-coordinate of the pixel to use for determining the
		 * background color.
		 * @param y The y-coordinate of the pixel to use for determining the
		 * background color.
		 * @param newColor The new background color to use for the texture.
		 * @param threshold The threshold value to use for comparing the pixel
		 * color to the background color. A higher threshold value will result
		 * in a larger area being considered as part of the background.
		 *
		 * @return A copy of the texture with the background removed and the new
		 * background color set to the specified color.
		 */
		static void removeTextureBackground(sf::Texture     &texture,
		                                    uint16_t         x,
		                                    u_int16_t        y,
		                                    const sf::Color &newColor  = sf::Color::Transparent,
		                                    double           threshold = 1.0);

		/**
		 * @brief Removes the background of the sprite managed by the
		 * SpriteManager and sets the new background color to the specified
		 * color.
		 *
		 * @param x The x-coordinate of the pixel to use for determining the
		 * background color.
		 * @param y The y-coordinate of the pixel to use for determining the
		 * background color.
		 * @param newColor The new background color to use for the sprite.
		 * @param threshold The threshold value to use for comparing the pixel
		 * color to the background color. A higher threshold value will result
		 * in a larger area being considered as part of the background.
		 *
		 * @return A copy of the sprite's texture with the background removed
		 * and the new background color set to the specified color.
		 */
		sf::Texture removeTextureBackground(uint16_t         x,
		                                    u_int16_t        y,
		                                    const sf::Color &newColor  = sf::Color::Transparent,
		                                    double           threshold = 1.0);

		/**
		 * @brief Removes the background of the sprite managed by the
		 * SpriteManager and sets the new background color to the specified
		 * color.
		 *
		 * @param pixelPosition The position of the pixel to use for determining
		 * the background color.
		 * @param newColor The new background color to use for the sprite.
		 * @param threshold The threshold value to use for comparing the pixel
		 * color to the background color. A higher threshold value will result
		 * in a larger area being considered as part of the background.
		 *
		 * @return A copy of the sprite's texture with the background removed
		 * and the new background color set to the specified color.
		 */
		sf::Texture removeTextureBackground(const sf::Vector2i &pixelPosition,
		                                    const sf::Color    &newColor  = sf::Color::Transparent,
		                                    double              threshold = 1.0);

		void                      SetFrames(const std::vector<sf::IntRect> &frames);
		std::vector<sf::IntRect> &GetFrames();

		void SetCurrentFrameIndex(int index);
		int  GetCurrentFrameIndex() const;

		sf::IntRect GetCurrentFrame() const;
		void        SetCurrentFrame(const sf::IntRect &rect);

		void DeleteFrame(int index);
		void AddFrame(const sf::IntRect &frame, int index = -1);

		void  SetAnimationTimer(float t);
		float GetAnimationTimer() const;

		void UpdateAnimation(float dt);
		void RenderAnimation(sf::RenderTarget &target);

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
