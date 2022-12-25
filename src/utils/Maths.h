#pragma once

#include "core/Logger.h"

namespace se {

	class Maths {
	  public:
		Maths(/* args */) {}
		~Maths() {}

		static sf::Color ImVec4toSFColor(ImVec4 color) {
			return sf::Color(color.x * 255, color.y * 255, color.z * 255, color.w * 255);
		}

		static float lerp(float a, float b, float f) { return a + f * (b - a); }

		static sf::Vector2f lerp(sf::Vector2f a, sf::Vector2f b, float f) { return a + f * (b - a); }

	  private:
		/* data */
	};
} // namespace se