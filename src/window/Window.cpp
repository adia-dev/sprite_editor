//
// Created by abdoulayedia on 16.12.2022
//
#include "Window.h"

#include "application/Application.h"

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
		_window = new sf::RenderWindow(sf::VideoMode(_width, _height), "ImGui SFML", sf::Style::Default);
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
		_imIO->FontGlobalScale = 1.25f;

		static const ImWchar icons_ranges[] = {ICON_MIN_FA, ICON_MAX_16_FA, 0};
		ImFontConfig         icons_config;
		icons_config.MergeMode  = true;
		icons_config.PixelSnapH = true;
		_imFont                 = _imIO->Fonts->AddFontFromFileTTF(
            "/Users/abdoulayedia/Projects/Dev/C++/sprite_editor/assets/fonts/FontAwesome/fa-solid-900.ttf",
            20.0f,
            &icons_config,
            icons_ranges);

		_imIO->Fonts->AddFontFromFileTTF("../assets/fonts/Operator-Mono/Fonts/OperatorMono-Medium.otf", 20.f);

		_imIO->IniFilename = "../src/imgui.ini";

		setFancyImguiStyle();

		if (!ImGui::SFML::UpdateFontTexture()) {
			std::cerr << "Could not update the font texture.\n";
			return -1;
		}

		if (!_shader.loadFromFile("../src/shaders/dashed_rectangle.frag", sf::Shader::Fragment)) {
			std::cerr << "Could not load the shader.\n";
			return -1;
		}

		_shader.setUniform("u_resolution", sf::Vector2f(640.0f, 480.0f));
		_shader.setUniform("u_borderWidth", 0.05f);
		_shader.setUniform("u_dashLength", 0.01f);

		return 1;
	}

	void Window::ShutDown() {
		if (_window == nullptr) return;
		_window->close();
	}

	const ImVec2 &Window::GetMousePos() const {
		return _mousePos;
	}

	const ImVec2 &Window::GetStartLeftMouseButtonPressedPos() const {
		return _startLeftMouseButtonPressedPos;
	}

	bool Window::GetIsLeftMouseButtonPressed() const {
		return _isLeftMouseButtonPressed;
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

		_mousePos = ImGui::GetIO().MousePos;

		// Store the starting mouse position when the mouse button is first pressed
		if (ImGui::IsMouseClicked(0)) {
			_startLeftMouseButtonPressedPos = ImGui::GetIO().MousePos;
			_isLeftMouseButtonPressed       = true;
		} else if (ImGui::IsMouseReleased(0) && _isLeftMouseButtonPressed) {
			_isLeftMouseButtonPressed = false;
		}

		ImGui::SFML::Update(*_window, _clock.restart());
	}

	void Window::Render() {
		ImGui::PushFont(_imFont);
		ImGui::DockSpaceOverViewport();
		ImGui::ShowDemoWindow();

		Components::MenuBar(_width);
		Components::Console();
		Components::ContentBrowser();
		Components::Hierarchy();
		Components::Properties();
		Components::Viewport();
		Components::Frames();
		Components::AnimationPreview();
		Components::AnimationTimeline();
		Components::ShaderEditor();
		Components::ShaderProperties();
		Components::OpenCVProperties();

		ImGui::PopStyleVar();
		ImGui::PopFont();

		_window->clear();
		ImGui::SFML::Render(*_window);
		_window->display();
	}

	// Static methods
	void Window::setFancyImguiStyle() {
		ImVec4 *colors                         = ImGui::GetStyle().Colors;
		colors[ImGuiCol_Text]                  = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
		colors[ImGuiCol_TextDisabled]          = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
		colors[ImGuiCol_WindowBg]              = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_PopupBg]               = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
		colors[ImGuiCol_Border]                = ImVec4(0.19f, 0.19f, 0.19f, 0.29f);
		colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.24f);
		colors[ImGuiCol_FrameBg]               = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_FrameBgActive]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_TitleBgActive]         = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
		colors[ImGuiCol_MenuBarBg]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
		colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_CheckMark]             = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_SliderGrab]            = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
		colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
		colors[ImGuiCol_Button]                = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
		colors[ImGuiCol_ButtonHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
		colors[ImGuiCol_ButtonActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_Header]                = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_HeaderHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
		colors[ImGuiCol_HeaderActive]          = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
		colors[ImGuiCol_Separator]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_SeparatorActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_ResizeGrip]            = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
		colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
		colors[ImGuiCol_Tab]                   = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabHovered]            = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_TabActive]             = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
		colors[ImGuiCol_TabUnfocused]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
		colors[ImGuiCol_DockingPreview]        = ImVec4(0.61f, 0.61f, 0.61f, 0.41f);
		colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_PlotLines]             = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
		colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.24f, 0.24f, 0.24f, 1.00f);
		colors[ImGuiCol_PlotHistogram]         = ImVec4(0.28f, 0.28f, 0.28f, 1.00f);
		colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
		colors[ImGuiCol_TableBorderLight]      = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
		colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
		colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
		colors[ImGuiCol_DragDropTarget]        = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
		colors[ImGuiCol_NavHighlight]          = ImVec4(0.23f, 0.23f, 0.23f, 1.00f);
		colors[ImGuiCol_NavWindowingHighlight] = ImVec4(0.35f, 0.35f, 0.35f, 0.70f);
		colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.36f, 0.36f, 0.36f, 0.20f);
		colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.34f, 0.34f, 0.34f, 0.35f);

		ImGuiStyle &style       = ImGui::GetStyle();
		style.WindowPadding     = ImVec2(8.00f, 8.00f);
		style.FramePadding      = ImVec2(20.00f, 10.00f);
		style.CellPadding       = ImVec2(6.00f, 6.00f);
		style.ItemSpacing       = ImVec2(6.00f, 6.00f);
		style.ItemInnerSpacing  = ImVec2(6.00f, 6.00f);
		style.TouchExtraPadding = ImVec2(0.00f, 0.00f);
		style.IndentSpacing     = 25;
		style.ScrollbarSize     = 15;
		style.GrabMinSize       = 10;
		style.WindowBorderSize  = 0;
		style.ChildBorderSize   = 0;
		style.PopupBorderSize   = 0;
		style.FrameBorderSize   = 0;
		style.TabBorderSize     = 1;
		style.WindowRounding    = 0;
		style.ChildRounding     = 4;
		style.FrameRounding     = 7;
		style.PopupRounding     = 12;
		style.ScrollbarRounding = 9;
		style.GrabRounding      = 3;
		style.LogSliderDeadzone = 4;
		style.TabRounding       = 4;

		style.WindowMenuButtonPosition = ImGuiDir_None;
	}

	sf::Color Window::ImVec4toSFColor(ImVec4 color) {
		return sf::Color(color.x * 255, color.y * 255, color.z * 255, color.w * 255);
	}
} // namespace se