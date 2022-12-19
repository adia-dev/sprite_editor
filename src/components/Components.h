#pragma once

#include "../utils/File.h"
#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"

// #include "../utils/Utils.h"
// #include "../utils/Logger.h"
// #include "../utils/Config.h"
// #include "../utils/AssetManager.h"
// #include "../utils/TextureManager.h"
// #include "../utils/Tileset.h"
// #include "../utils/Tilemap.h"
// #include "../utils/TilemapEditor.h"

class Application;

namespace se
{
	class Components
	{
	public:
		Components() { }
		~Components() { }

		/* Components */
		static void MenuBar(uint16_t windowWidth);
		static void Console(const std::vector<std::string>& logs);
		static void ContentBrowser(std::filesystem::path& path);
		static void Hierarchy();
		static void Properties();

	private:
		static void HelpMarker(const char* desc);

		static std::string exec(const std::string& cmd)
		{
			char buffer[128];
			std::string result;
			std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd.c_str(), "r"), pclose);
			if(!pipe)
			{
				throw std::runtime_error("popen() failed!");
			}
			while(fgets(buffer, strlen(buffer), pipe.get()) != nullptr)
			{
				result += buffer;
			}
			return result;
		}
	};

} // namespace se