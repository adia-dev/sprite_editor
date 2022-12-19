
#include "AssetManager.h"

namespace se {
	sf::Texture& AssetManager::GetTexture(const std::string& name) {
		auto& map  = AssetManager::Get()._textures;
		auto  find = map.find(name);

		if (find != map.end()) {
			return find->second;
		}

		map.emplace(name, sf::Texture());
		map[name].loadFromFile(name);
		return map[name];
	}
} // namespace se
