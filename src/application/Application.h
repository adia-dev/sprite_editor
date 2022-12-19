//
// Created by abdoulayedia on 16.12.2022
//
#pragma once

#include "../window/Window.h"

namespace se
{
	// Application
	class Application
	{
	public:
		static Application& Get()
		{
			static Application instance;
			return instance;
		}

		int Run();
		void Quit();

	private:
		Application();
		~Application();

		std::shared_ptr<Window> _window = nullptr;

	public:
		Application(Application const&) = delete;
		Application& operator=(const Application&) = delete;
	};
} // namespace se
