//
// Created by abdoulayedia on 16.12.2022
//
#include "Application.h"

namespace se {

	Application::Application() {
		_window           = std::make_shared<Window>();
		_currentDirectory = __CURRENT_DIR__;
		_spriteManager.LoadSprite(
		    "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/assets/"
		    "images/"
		    "spritesheets/goku/1.png");
	}

	Application::~Application() {}

	void Application::LoadEditorIcons() {
		if (!AssetManager::Get().LoadTexture(_editorIconsPath)) {
		}

		_editorIconsRects["play"] = sf::IntRect(0, 0, 32, 32);
		_editorIconsRects["stop"] = sf::IntRect(32, 0, 32, 32);
	}

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

	sf::Sprite Application::GetEditorIcon(const std::string& name) {
		if (_editorIconsRects.find(name) == _editorIconsRects.end())
			return sf::Sprite();

		sf::Sprite sprite;
		sprite.setTexture(AssetManager::Get().GetTexture(_editorIconsPath));
		sprite.setTextureRect(_editorIconsRects[name]);
		return sprite;
	}

} // namespace se
