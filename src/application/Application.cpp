//
// Created by abdoulayedia on 16.12.2022
//
#include "Application.h"

namespace se {

	Application::Application() {
		_window           = std::make_shared<Window>();
		_currentDirectory = __CURRENT_DIR__;
		_spriteManager.LoadSprite(__DEFAULT_SPRITE__);

		LoadEditorIcons();
	}

	Application::~Application() {}

	void Application::LoadEditorIcons() {
		if (!AssetManager::Get().LoadTexture(_editorIconsPath)) {
			Logger::Get().Error("Failed to load editor icons", __FILE__, __LINE__);
			return;
		}

		// for (int i = 0; i < 15; i++) {
		// 	for (int j = 0; j < 11; j++) {
		// 		r.setPosition(200 * i + 100, 200 * j + 94);
		// 		r.setSize(sf::Vector2f(200, 200));
		// 		window.draw(r);
		// 	}
		// }

		const uint16_t iconSize = 200;
		const uint16_t offsetX  = 100;
		const uint16_t offsetY  = 94;

		_editorIconsRects["pen"] = sf::IntRect(offsetX, offsetY, iconSize, iconSize);
		_editorIconsRects["add_rect"] =
		    sf::IntRect(offsetX + iconSize * 13, offsetY + iconSize * 2, iconSize, iconSize);
		_editorIconsRects["magic_wand"] =
		    sf::IntRect(offsetX + iconSize * 8, offsetY + iconSize * 2, iconSize, iconSize);
	}

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

	sf::Sprite Application::GetEditorIcon(const std::string& name, const sf::Vector2f& size) {
		if (_editorIconsRects.find(name) == _editorIconsRects.end()) return sf::Sprite();

		sf::Sprite sprite;
		sprite.setTexture(AssetManager::Get().GetTexture(_editorIconsPath));
		sprite.setScale(5.f, 5.f);
		sprite.setTextureRect(_editorIconsRects[name]);
		return sprite;
	}

} // namespace se
