#pragma once

#include "utils/File.h"

#include <map>
#include <string>
#include <unordered_map>

namespace se {

	class AssetManager {
	  public:
		static AssetManager& Get() {
			static AssetManager instance;
			return instance;
		}

		sf::Texture& GetTexture(const std::string& name);

	  private:
		AssetManager() {}
		~AssetManager() {}

		std::map<std::string, sf::Texture> _textures;

	  public:
		AssetManager(AssetManager const&)            = delete;
		AssetManager& operator=(const AssetManager&) = delete;
	};

} // namespace se
