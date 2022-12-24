//
// Created by abdoulayedia on 16.12.2022
//
#include "Application.h"

namespace se {

	Application::Application() {
		_window           = std::make_shared<Window>();
		_currentDirectory = __CURRENT_DIR__;
		_spriteManager.LoadSprite(
		    "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/assets/images/"
		    "spritesheets/vegito/ssj_blue.png");
	}

	Application::~Application() {}

	int Application::Run() {
		if (_window == nullptr) return 1;

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

	SpriteManager& Application::GetSpriteManager() {
		return _spriteManager;
	}

} // namespace se
