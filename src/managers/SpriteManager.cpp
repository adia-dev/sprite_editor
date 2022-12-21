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
	SpriteManager::SliceSprite(const std::string& filepath) {
		cv::Mat                  original, img, gray, bw;
		cv::Mat                  close, dil;
		std::vector<sf::IntRect> slices;

		original = cv::imread(cv::samples::findFile(filepath));
		original.copyTo(img);
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		double thresh = cv::threshold(gray,
		                              bw,
		                              0,
		                              255,
		                              cv::THRESH_BINARY | cv::THRESH_OTSU);

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});
		morphologyEx(bw, close, cv::MORPH_CLOSE, kernel, {-1, -1}, 2);
		dilate(close, dil, kernel, {-1, -1}, 2);

		std::vector<std::vector<cv::Point>> contours;
		findContours(dil, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		int spriteCount = 0;

		for (auto c : contours) {
			cv::Rect bRect = boundingRect(c);
			cv::Mat  roi   = img(bRect);
			double   area  = contourArea(c);
			if (area < 100.0) continue;

			slices.push_back({bRect.x, bRect.y, bRect.width, bRect.height});
		}

		// imshow("thresh", bw);
		// imshow("dilate", dil);
		// imshow("image", img);

		return slices;
	}

	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const std::string& filepath,
	                           const sf::IntRect& roiRect) {
		cv::Mat                  original, img, gray, bw;
		cv::Mat                  close, dil;
		std::vector<sf::IntRect> slices;

		original = cv::imread(cv::samples::findFile(filepath));
		img      = original(
            cv::Rect(roiRect.left, roiRect.top, roiRect.width, roiRect.height));
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		double thresh = cv::threshold(gray,
		                              bw,
		                              0,
		                              255,
		                              cv::THRESH_BINARY | cv::THRESH_OTSU);

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});
		morphologyEx(bw, close, cv::MORPH_CLOSE, kernel, {-1, -1}, 2);
		dilate(close, dil, kernel, {-1, -1}, 2);

		std::vector<std::vector<cv::Point>> contours;
		findContours(dil, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		int spriteCount = 0;

		for (auto c : contours) {
			cv::Rect bRect = boundingRect(c);
			cv::Mat  roi   = img(bRect);
			double   area  = contourArea(c);
			if (area < 100.0) continue;

			slices.push_back({bRect.x, bRect.y, bRect.width, bRect.height});
		}

		// imshow("thresh", bw);
		// imshow("dilate", dil);
		// imshow("image", img);

		return slices;
	}

	std::vector<sf::IntRect>
	SpriteManager::SliceSprite(const sf::Texture& texture,
	                           const sf::IntRect& roiRect) {
		cv::Mat                  original, img, gray, bw;
		cv::Mat                  close, dil;
		std::vector<sf::IntRect> slices;

		original = TextureToOpenCVMat(texture);
		img      = original(
            cv::Rect(roiRect.left, roiRect.top, roiRect.width, roiRect.height));
		cvtColor(img, gray, cv::COLOR_BGR2GRAY);

		double thresh = cv::threshold(gray,
		                              bw,
		                              0,
		                              255,
		                              cv::THRESH_BINARY | cv::THRESH_OTSU);

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, {3, 3});
		morphologyEx(bw, close, cv::MORPH_CLOSE, kernel, {-1, -1}, 2);
		dilate(close, dil, kernel, {-1, -1}, 2);

		std::vector<std::vector<cv::Point>> contours;
		findContours(dil, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

		int spriteCount = 0;

		for (auto c : contours) {
			cv::Rect bRect = boundingRect(c);
			cv::Mat  roi   = img(bRect);
			double   area  = contourArea(c);
			if (area < 100.0) continue;

			slices.push_back({bRect.x, bRect.y, bRect.width, bRect.height});
		}

		// imshow("thresh", bw);
		// imshow("dilate", dil);
		// imshow("image", img);

		return slices;
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