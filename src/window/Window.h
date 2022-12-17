//
// Created by abdoulayedia on 16.12.2022
//
#pragma once

#include "imgui-SFML.h"
#include "imgui.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <iostream>

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

	private:
		uint16_t _width  = 2560;
		uint16_t _height = 1600;

		// SFML variables
		sf::RenderWindow* _window = nullptr;
		sf::RenderTexture _renderTexture;
		sf::Clock         _clock;

		// ImGui variables
		ImFont*  _imFont = nullptr;
		ImGuiIO* _imIO   = nullptr;

		// Init methods
		int Init();

	public:
		static void      setFancyImguiStyle();
		static sf::Color ImVec4toSFColor(ImVec4 color);
	};
} // namespace se
