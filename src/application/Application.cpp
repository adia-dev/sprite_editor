//
// Created by abdoulayedia on 16.12.2022
//
#include "Application.h"

namespace se {

	Application::Application() {
		_window           = std::make_shared<Window>();
		_currentDirectory = __CURRENT_DIR__;
		_spriteManager.LoadSprite(__DEFAULT_SPRITE__);
	}

	Application::~Application() {}

	int Application::Run() {
		if (_window == nullptr) return 1;

		Logger::Get().Info("Starting application", __FILE__, __LINE__);
		Logger::Get().Error("Error", __FILE__, __LINE__);
		Logger::Get().Fatal("Fatal", __FILE__, __LINE__);
		Logger::Get().Warn("Warn", __FILE__, __LINE__);

		while (_window->isOpen()) {
			_window->HandleEvents();
			_window->Update();
			_window->Render();
		}

		return 0;
	}

	void Application::Quit() {
		if (_window != nullptr) _window->ShutDown();
	}

	std::filesystem::path& Application::GetCurrentDirectory() {
		return _currentDirectory;
	}

	void Application::SetCurrentDirectory(const std::filesystem::path& path) {
		_currentDirectory = path;
	}

	sf::Clock& Application::GetClock() {
		return _clock;
	}

	std::chrono::system_clock::time_point& Application::GetStartTime() {
		return _start;
	}

	std::time_t Application::Now() {
		return std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	}

	std::string Application::NowStr() {
		return TimeToString(Now());
	}

	SpriteManager& Application::GetSpriteManager() {
		return _spriteManager;
	}

	Window& Application::GetWindow() {
		return *_window.get();
	}

} // namespace se
