//
// Created by abdoulayedia on 01.01.2023
//
#pragma once

#include "core/Logger.h"

namespace se {
	// Serializer
	class Serializer {
	  public:
		Serializer()  = default;
		~Serializer() = default;

		// Serialize
		static void SaveFramesToXML(const std::filesystem::path& path, const std::vector<sf::IntRect>& frames);
		static std::vector<sf::IntRect> LoadFramesFromXML(const std::filesystem::path& path);
	};
} // namespace se
