//
// Created by abdoulayedia on 16.12.2022
//
#pragma once

#include "components/Components.h"

namespace se {
	// Window
	class Window {
	  public:
		Window();
		~Window();

		bool isOpen() const { return _window != nullptr && _window->isOpen(); }

		void HandleEvents();
		void Update();
		void Render();
		void ShutDown();

	  private:
		uint16_t _width  = 2560;
		uint16_t _height = 1600;

		// SFML variables
		sf::RenderWindow*        _window = nullptr;
		sf::RenderTexture        _renderTexture;
		sf::Clock                _clock;
		std::vector<sf::IntRect> _boundingRects;

		// ImGui variables
		ImFont*   _imFont         = nullptr;
		ImGuiIO*  _imIO           = nullptr;
		u_int16_t _imageTextureId = 0;

		// Init methods
		int Init();

	  public:
		static void      setFancyImguiStyle();
		static sf::Color ImVec4toSFColor(ImVec4 color);
	};
} // namespace se
