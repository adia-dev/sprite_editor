#pragma once

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

// #include "../utils/Utils.h"
// #include "../utils/Logger.h"
// #include "../utils/Config.h"
// #include "../utils/AssetManager.h"
// #include "../utils/TextureManager.h"
// #include "../utils/Tileset.h"
// #include "../utils/Tilemap.h"
// #include "../utils/TilemapEditor.h"

namespace se {
	class Components {
	  public:
		Components() {}
		~Components() {}

		/* Components */
		static void MenuBar(uint16_t windowWidth);
		static void Console(const std::vector<std::string>& logs);

	  private:
		static void HelpMarker(const char* desc);
	};

} // namespace se