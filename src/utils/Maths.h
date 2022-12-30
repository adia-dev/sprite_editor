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

		static sf::Color lerp(sf::Color a, sf::Color b, float f) {
			return sf::Color(lerp(a.r, b.r, f), lerp(a.g, b.g, f), lerp(a.b, b.b, f), lerp(a.a, b.a, f));
		}

		static sf::Color ImColorToSFMLColor(const ImColor& color) {
			return sf::Color(color.Value.x * 255, color.Value.y * 255, color.Value.z * 255, color.Value.w * 255);
		}

	  private:
		/* data */
	};
} // namespace se