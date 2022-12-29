
#include "AssetManager.h"

namespace se {
	sf::Texture& AssetManager::GetTexture(const std::string& name) {
		auto& map  = AssetManager::Get()._textures;
		auto  find = map.find(name);

		if (find != map.end()) {
			return find->second;
		}

		map.emplace(name, sf::Texture());
		if (!map[name].loadFromFile(name)) {
			std::cout << "Failed to load texture: " << name << std::endl;
		}
		return map[name];
	}
	bool AssetManager::HasTexture(const std::string& name) {
		auto& map  = AssetManager::Get()._textures;
		auto  find = map.find(name);

		if (find != map.end()) {
			return true;
		}
		return false;
	}
	bool AssetManager::LoadTexture(const std::string& name) {
		auto& map  = AssetManager::Get()._textures;
		auto  find = map.find(name);

		if (find != map.end()) {
			return true;
		}

		map.emplace(name, sf::Texture());
		if (!map[name].loadFromFile(name)) {
			std::cout << "Failed to load texture: " << name << std::endl;
			return false;
		}
		return true;
	}
} // namespace se
