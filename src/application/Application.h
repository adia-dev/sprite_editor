//
// Created by abdoulayedia on 16.12.2022
//
#pragma once

#include "window/Window.h"

namespace se {
	// Application
	class Application {
	  public:
		static Application& Get() {
			static Application instance;
			return instance;
		}

		int  Run();
		void Quit();

		std::filesystem::path& GetCurrentDirectory();
		void SetCurrentDirectory(const std::filesystem::path& path);

		SpriteManager& GetSpriteManager();
		sf::Sprite     GetEditorIcon(const std::string& name);

	  private:
		Application();
		~Application();

		std::shared_ptr<Window> _window = nullptr;
		SpriteManager           _spriteManager;
		std::filesystem::path   _currentDirectory;

		std::unordered_map<std::string, sf::IntRect> _editorIconsRects;
		std::string _editorIconsPath = "../resources/icons/icons_pack_bg.png";

		void LoadEditorIcons();

	  public:
		Application(Application const&)            = delete;
		Application& operator=(const Application&) = delete;
	};
} // namespace se
