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
		static void MenuBar();
		static void Console();
		static void ContentBrowser();
		static void Hierarchy();
		static void Properties();
		// static void Inspector();
		static void AnimationPreview();
		static void Viewport();
		static void Frames();
		static void ShaderEditor();
		static void ShaderProperties();

		static void RenderDashedRectangle(sf::RenderTarget  &target,
		                                  sf::Shader        &shader,
		                                  const sf::IntRect &rect,
		                                  const sf::Color   &color = sf::Color::White);

	  private:
		static void HelpMarker(const char *desc);
	};

} // namespace se
