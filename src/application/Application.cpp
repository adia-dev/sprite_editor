//
// Created by abdoulayedia on 16.12.2022
//
#include "Application.h"

namespace se {

	Application::Application() {
		_window = std::make_shared<Window>();
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

} // namespace se
