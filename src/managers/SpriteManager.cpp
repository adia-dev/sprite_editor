//
// Created by abdoulayedia on 20.12.2022
//
#include "SpriteManager.h"

#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

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
} // namespace se