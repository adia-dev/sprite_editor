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

	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const sf::IntRect& roiRect) {
		return SpriteManager::SliceSprite(*_sprite.getTexture(), roiRect);
	}

	/**
	 * @brief Slices the given texture into individual sprite images using
	 * OpenCV.
	 *
	 * The region of interest rectangle (roiRect) allows you to specify a
	 * specific area within the texture to slice. This can be useful if the
	 * texture contains multiple sprite sheets or other unwanted content.
	 *
	 * @param texture The texture to slice.
	 * @param roiRect The region of interest rectangle within the texture.
	 * @return A vector of rectangles representing the bounding boxes of the
	 * individual sprite images.
	 */
	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const sf::Texture& texture,
	                           const sf::IntRect& roiRect) {
		// Declare OpenCV matrices and a vector of rectangles
		cv::Mat                  original, img, gray, bw;
		cv::Mat                  close, dil, blurred;
		std::vector<sf::IntRect> slices;

		// Convert the texture to an OpenCV matrix
		original = TextureToOpenCVMat(texture);

		// Get a region of interest matrix from the original matrix using the
		// roiRect
		img = original(
		    cv::Rect(roiRect.left, roiRect.top, roiRect.width, roiRect.height));

		// Convert the region of interest matrix to grayscale
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		cv::GaussianBlur(gray, blurred, {3, 3}, 0);

		// Apply Otsu's threshold to the grayscale matrix to create a black and
		// white image
		double thresh = cv::threshold(blurred, bw, 0, 255, cv::THRESH_BINARY);

		// Create a 3x3 rectangular kernel for morphological transformations
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});

		// Apply a closing morphological transformation to the black and white
		// image
		morphologyEx(bw, close, cv::MORPH_CLOSE, kernel, {-1, -1}, 2);

		// Dilate the closing image
		dilate(close, dil, kernel, {-1, -1}, 2);

		cv::GaussianBlur(dil, blurred, {3, 3}, 0);

		cv::morphologyEx(blurred,
		                 blurred,
		                 cv::MORPH_CLOSE,
		                 kernel,
		                 {-1, -1},
		                 2);

		// Find the contours in the dilated image
		std::vector<std::vector<cv::Point>> contours;
		findContours(blurred,
		             contours,
		             cv::RETR_EXTERNAL,
		             cv::CHAIN_APPROX_SIMPLE);

		// Iterate through the contours
		int spriteCount = 0;
		for (auto c : contours) {
			// Get the bounding rectangle of the contour
			cv::Rect bRect = boundingRect(c);

			// Calculate the area of the contour
			double area = contourArea(c);

			// Skip contours with small areas
			if (area < 120.0) continue;

			// Add the bounding rectangle to the vector of rectangles
			slices.push_back({bRect.x, bRect.y, bRect.width, bRect.height});
		}

		// imshow("gray", gray);
		// imshow("thresh", bw);
		// imshow("close", close);
		// imshow("dilate", dil);
		// imshow("blurred", blurred);

		// Return the vector of rectangles
		return slices;
	}

	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const std::string& filepath) {
		sf::Texture texture;
		texture.loadFromFile(filepath);

		return SliceSprite(
		    texture,
		    sf::IntRect(0, 0, texture.getSize().x, texture.getSize().y));
	}

	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const std::string& filepath,
	                           const sf::IntRect& roiRect) {
		sf::Texture texture;
		texture.loadFromFile(filepath);

		return SliceSprite(texture, roiRect);
	}

	cv::Mat SpriteManager::TextureToOpenCVMat(const sf::Texture& texture) {
		sf::Image img = texture.copyToImage();

		uint16_t width  = img.getSize().x;
		uint16_t height = img.getSize().y;

		const sf::Uint8* pixels = img.getPixelsPtr();

		return cv::Mat(height, width, CV_8UC4, (void*)pixels);
	}

	void SpriteManager::removeTextureBackground(sf::Texture&     texture,
	                                            uint16_t         x,
	                                            u_int16_t        y,
	                                            const sf::Color& newColor,
	                                            double           threshold) {
		sf::Image img = texture.copyToImage();

		uint16_t width  = img.getSize().x;
		uint16_t height = img.getSize().y;

		if (x < 0 || x >= width || y < 0 || y >= height) return;

		sf::Color colorToReplace = img.getPixel(x, y);

		for (uint16_t i = 0; i < height; ++i) {
			for (uint16_t j = 0; j < width; ++j) {
				// Calculate the Euclidean distance between the colorToReplace
				// and the pixel color
				int    r        = colorToReplace.r - img.getPixel(j, i).r;
				int    g        = colorToReplace.g - img.getPixel(j, i).g;
				int    b        = colorToReplace.b - img.getPixel(j, i).b;
				double distance = sqrt(r * r + g * g + b * b);

				// If the distance is below the threshold, set the pixel color
				// to the new color
				if (distance < threshold) {
					img.setPixel(j, i, newColor);
				}
			}
		}

		texture.loadFromImage(img);
	}

	sf::Texture
	SpriteManager::removeTextureBackground(uint16_t         x,
	                                       u_int16_t        y,
	                                       const sf::Color& newColor,
	                                       double           threshold) {
		const sf::Texture* spriteTex = _sprite.getTexture();
		if (spriteTex == nullptr) return {};

		sf::Image img = spriteTex->copyToImage();

		uint16_t width  = img.getSize().x;
		uint16_t height = img.getSize().y;

		if (x < 0 || x >= width || y < 0 || y >= height) return {};

		sf::Color colorToReplace = img.getPixel(x, y);

		for (uint16_t i = 0; i < height; ++i) {
			for (uint16_t j = 0; j < width; ++j) {
				// Calculate the Euclidean distance between the colorToReplace
				// and the pixel color
				int    r        = colorToReplace.r - img.getPixel(j, i).r;
				int    g        = colorToReplace.g - img.getPixel(j, i).g;
				int    b        = colorToReplace.b - img.getPixel(j, i).b;
				double distance = sqrt(r * r + g * g + b * b);

				// If the distance is below the threshold, set the pixel color
				// to the new color
				if (distance < threshold) {
					img.setPixel(j, i, newColor);
				}
			}
		}

		sf::Texture newTexture;
		newTexture.loadFromImage(img);

		return newTexture;
	}

	sf::Texture
	SpriteManager::removeTextureBackground(const sf::Vector2i& pixelPosition,
	                                       const sf::Color&    newColor,
	                                       double              threshold) {
		return removeTextureBackground(pixelPosition.x,
		                               pixelPosition.y,
		                               newColor,
		                               threshold);
	}

} // namespace se