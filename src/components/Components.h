#pragma once

#include "managers/SpriteManager.h"

// #include "../utils/Utils.h"
// #include "../utils/Logger.h"
// #include "../utils/Config.h"
// #include "../utils/AssetManager.h"
// #include "../utils/TextureManager.h"
// #include "../utils/Tileset.h"
// #include "../utils/Tilemap.h"
// #include "../utils/TilemapEditor.h"

class Application;

namespace se {
	class Components {
	  public:
		Components() {}
		~Components() {}

		/* Components */
		static void MenuBar(uint16_t windowWidth);
		static void Console(const std::vector<std::string>& logs);
		static void ContentBrowser();
		static void Hierarchy();
		static void Properties();
		// static void Inspector();
		static void AnimationPreview(const std::vector<sf::IntRect>& frames);

	  private:
		static void HelpMarker(const char* desc);
	};

} // namespace se
