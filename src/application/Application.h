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
		void                   SetCurrentDirectory(const std::filesystem::path& path);
		// get the running time of the application
		sf::Clock& GetClock();

		std::chrono::system_clock::time_point& GetStartTime();
		static std::time_t                     Now();
		static std::string                     NowStr();

		static std::string TimeToString(std::time_t time) {
			std::stringstream dateStr;
			dateStr << std::put_time(std::localtime(&time), "%H:%M:%S");
			return dateStr.str();
		}

		SpriteManager& GetSpriteManager();
		Window&        GetWindow();

	  private:
		Application();
		~Application();

		std::shared_ptr<Window>               _window = nullptr;
		SpriteManager                         _spriteManager;
		std::filesystem::path                 _currentDirectory;
		sf::Clock                             _clock;
		std::chrono::system_clock::time_point _start = std::chrono::system_clock::now();

	  public:
		Application(Application const&)            = delete;
		Application& operator=(const Application&) = delete;
	};
} // namespace se
