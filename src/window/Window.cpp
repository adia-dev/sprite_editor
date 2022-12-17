//
// Created by abdoulayedia on 16.12.2022
//
#include "Window.h"

namespace se {
	Window::Window() {
		Init();
	}

	Window::~Window() {
		if (_window) {
			delete _window;
		}
	}

	int Window::Init() {
		_window = new sf::RenderWindow(sf::VideoMode(_width, _height),
		                               "ImGui SFML",
		                               sf::Style::Default);
		if (_window == nullptr) {
			std::cerr << "Could not create the window instance.\n";
			return -1;
		}

		if (!ImGui::SFML::Init(*_window)) {
			std::cerr << "Could not init the ImGui SFML window.\n";
			return -1;
		}

		_imIO = &ImGui::GetIO();
		_imIO->ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		_imIO->FontGlobalScale = 1.5f;

		_imFont = _imIO->Fonts->AddFontFromFileTTF(
		    "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/assets/fonts/"
		    "Poppins/Poppins-Regular.ttf",
		    20);

		setFancyImguiStyle();

		if (!ImGui::SFML::UpdateFontTexture()) {
			std::cerr << "Could not update the font texture.\n";
			return -1;
		}

		return 1;
	}

	void Window::HandleEvents() {
		if (_window == nullptr) return;

		sf::Event event;
		while (_window->pollEvent(event)) {
			ImGui::SFML::ProcessEvent(*_window, event);

			switch (event.type) {
				case sf::Event::Closed:
					_window->close();
					break;
				case sf::Event::KeyPressed:
					if (event.key.code == sf::Keyboard::Escape) {
						_window->close();
					}
					break;
				default:
					break;
			}
		}
	}

	void Window::Update() {
		if (_window == nullptr) return;

		ImGui::SFML::Update(*_window, _clock.restart());
	}

	void Window::Render() {
		static ImVec4      circleColor {1.0f, 0.0f, 1.0f, 0.5f};
		static float       circleRadius = 100.f;
		static std::size_t circlePoints = 30;

		sf::CircleShape shape {circleRadius, circlePoints};
		shape.setFillColor(ImVec4toSFColor(circleColor));

		static ImVec2     viewportSize {500, 500};
		sf::RenderTexture rt {};
		rt.create(viewportSize.x, viewportSize.y);

		rt.clear(sf::Color::Black);
		rt.draw(shape);

		ImGui::PushFont(_imFont);
		ImGui::DockSpaceOverViewport();

		ImGui::ShowDemoWindow();

		if (ImGui::Begin("Circle manipulator")) {
			ImGuiColorEditFlags flags = ImGuiColorEditFlags_AlphaBar;
			ImGui::ColorPicker4("Circle Color",
			                    (float *)&circleColor,
			                    flags,
			                    nullptr);

			ImGui::DragFloat("Circle Radius", &circleRadius);
			ImGui::DragInt("Circle Points", (int *)&circlePoints, 1.0f, 3, 500);
		}
		ImGui::End();

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		if (ImGui::Begin("Viewport")) {
			viewportSize = ImGui::GetWindowSize();
			ImGui::Image(rt);
		}
		ImGui::End();
		ImGui::PopStyleVar();
		ImGui::PopFont();

		_window->clear();
		_window->draw(shape);
		ImGui::SFML::Render(*_window);
		_window->display();
	}

	// Static methods
	void Window::setFancyImguiStyle() {
		ImGui::GetStyle().FrameRounding = 4.0f;
		ImGui::GetStyle().GrabRounding  = 4.0f;

		ImGui::GetStyle().FramePadding     = ImVec2(6, 4);
		ImGui::GetStyle().ItemSpacing      = ImVec2(6, 2);
		ImGui::GetStyle().ItemInnerSpacing = ImVec2(6, 4);
		ImGui::GetStyle().IndentSpacing    = 6.0f;

		ImVec4 *colors                         = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text]                  = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
		colors[ImGuiCol_TextDisabled]          = ImVec4(0.36f, 0.42f, 0.47f, 1.00f);
		colors[ImGuiCol_WindowBg]              = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_ChildBg]               = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_PopupBg]               = ImVec4(0.08f, 0.08f, 0.08f, 0.94f);
		colors[ImGuiCol_Border]                = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.12f, 0.20f, 0.28f, 1.00f);
		colors[ImGuiCol_FrameBgActive]         = ImVec4(0.09f, 0.12f, 0.14f, 1.00f);
		colors[ImGuiCol_TitleBg]               = ImVec4(0.09f, 0.12f, 0.14f, 0.65f);
		colors[ImGuiCol_TitleBgActive]         = ImVec4(0.08f, 0.10f, 0.12f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
		colors[ImGuiCol_MenuBarBg]             = ImVec4(0.15f, 0.18f, 0.22f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.02f, 0.02f, 0.02f, 0.39f);
		colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.18f, 0.22f, 0.25f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.09f, 0.21f, 0.31f, 1.00f);
		colors[ImGuiCol_CheckMark]             = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrab]            = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.37f, 0.61f, 1.00f, 1.00f);
		colors[ImGuiCol_Button]                = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_ButtonHovered]         = ImVec4(0.28f, 0.56f, 1.00f, 1.00f);
		colors[ImGuiCol_ButtonActive]          = ImVec4(0.06f, 0.53f, 0.98f, 1.00f);
		colors[ImGuiCol_Header]                = ImVec4(0.20f, 0.25f, 0.29f, 0.55f);
		colors[ImGuiCol_HeaderHovered]         = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_HeaderActive]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_Separator]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
		colors[ImGuiCol_SeparatorActive]       = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
		colors[ImGuiCol_ResizeGrip]            = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
		colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
		colors[ImGuiCol_Tab]                   = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabHovered]            = ImVec4(0.26f, 0.59f, 0.98f, 0.80f);
		colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.25f, 0.29f, 1.00f);
		colors[ImGuiCol_TabUnfocused]          = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.11f, 0.15f, 0.17f, 1.00f);
		colors[ImGuiCol_PlotLines]             = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]      = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
		colors[ImGuiCol_PlotHistogram]         = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
		colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
		colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
		colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
	}

	sf::Color Window::ImVec4toSFColor(ImVec4 color) {
		return sf::Color(color.x * 255,
		                 color.y * 255,
		                 color.z * 255,
		                 color.w * 255);
	}
} // namespace se