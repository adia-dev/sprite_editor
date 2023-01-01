#include "Components.h"

#include "application/Application.h"

namespace se {

	void Components::MenuBar() {
		ImGuiViewportP* viewport = (ImGuiViewportP*)(void*)ImGui::GetMainViewport();

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
		                                ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

		float height = ImGui::GetFrameHeight();

		// Main menu bar
		if (ImGui::BeginViewportSideBar("##MainMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
			if (ImGui::BeginMenuBar()) {
				if (ImGui::BeginMenu("File")) {
					if (ImGui::MenuItem("New")) {
						std::cout << File::Dialog() << std::endl;
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
					}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {
					}
					if (ImGui::MenuItem("Save As...")) {
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit")) {
						Application::Get().Quit();
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Edit")) {
					if (ImGui::MenuItem("Undo", "Ctrl+Z")) {
					}
					if (ImGui::MenuItem("Redo", "Ctrl+Y", false,
					                    false)) { // Disabled item
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Cut", "Ctrl+X")) {
					}
					if (ImGui::MenuItem("Copy", "Ctrl+C")) {
					}
					if (ImGui::MenuItem("Paste", "Ctrl+V")) {
					}
					ImGui::EndMenu();
				}
				if (ImGui::BeginMenu("Render")) {
					if (ImGui::MenuItem("Render to file")) {
						Logger::Get().Info("Render to file");
					}
					ImGui::EndMenu();
				}
				// Window
				if (ImGui::BeginMenu("Window")) {
					if (ImGui::MenuItem("Console")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Content Browser")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Hierarchy")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Properties")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Animation Preview")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Animation Timeline")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Viewport")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Frames")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Shader Editor")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Shader Properties")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("OpenCV Properties")) {
						Logger::Get().Info("Toggle: XXX");
					}
					if (ImGui::MenuItem("Toolbox")) {
						Logger::Get().Info("Toggle: XXX");
					}
					ImGui::EndMenu();
				}

				// Help
				if (ImGui::BeginMenu("Help")) {
					if (ImGui::MenuItem("About")) {
						Logger::Get().Info("Toggle: XXX");
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

		// if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
		// 	if (ImGui::BeginMenuBar()) {
		// 		if (ImGui::Button(Application::Get().GetWindow().GetShowGrid() ? ICON_FA_RECTANGLE_XMARK
		// 		                                                               : ICON_FA_SQUARE)) {
		// 			Application::Get().GetWindow().ToggleShowGrid();
		// 		}
		// 		ImGui::SameLine();
		// 		ImGui::Button("Game");
		// 		ImGui::SameLine();
		// 		ImGui::Button("Scene");
		// 		ImGui::SameLine();

		// 		ImGui::EndMenuBar();
		// 	}

		// 	ImGui::End();
		// }

		if (ImGui::BeginViewportSideBar("##MainStatusBar", viewport, ImGuiDir_Down, height, window_flags)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::Text("Status bar, project name, unsaved changes, etc.");
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	}

	void Components::Console() {
		ImGui::Begin("Console");
		static char searchBuffer[256] = {0};
		const auto& logs              = Logger::Get().GetLogs();

		// Inline buttons and a search bar
		{
			ImGui::BeginChild("Buttons", ImVec2(0, 50));
			ImGui::SameLine();
			if (ImGui::Button("Clear")) {
				Logger::Get().Clear();
			}
			ImGui::SameLine();
			if (ImGui::Button("Save")) {
				// Save the log
				std::cout << "Saving the log" << std::endl;
			}

			ImGui::SameLine();
			// Input text with a placeholder and a clear button
			ImGui::InputTextWithHint("##Search",
			                         "Search...",
			                         searchBuffer,
			                         IM_ARRAYSIZE(searchBuffer),
			                         ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue);
			ImGui::SameLine();
			if (ImGui::Button("x", ImVec2(37, 37))) {
				searchBuffer[0] = '\0';
			}

			ImGui::EndChild();
		}

		ImGui::BeginChild("ScrollingRegion",
		                  ImVec2(0, -ImGui::GetFrameHeightWithSpacing()),
		                  false,
		                  ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
		for (int i = 0; i < logs.size(); i++) {
			// filter logs if searchBuffer is not empty
			if (searchBuffer[0] != '\0') {
				if (logs[i].message.find(searchBuffer) == std::string::npos) {
					continue;
				}
			}

			ImGui::PushID(i);
			// Clickable text, with action such has right click opens a menu with copy, etc.
			if (ImGui::Selectable("##LogSelectable", false, 0, ImVec2(0, 80))) {
			}
			ImGui::SameLine();
			ImGui::BeginGroup();
			{
				ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f),
				                   "[%s]",
				                   Application::TimeToString(logs[i].time).c_str());
				ImGui::SameLine();
				ImGui::TextColored(Logger::GetColor(logs[i].level), "%s", logs[i].message.c_str());
				if (!logs[i].file.empty()) {
					ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "[%s:%d]", logs[i].file.c_str(), logs[i].line);
				}
			}
			ImGui::EndGroup();

			// Right click on text
			if (ImGui::BeginPopupContextItem("##LogPopup")) {
				if (ImGui::Selectable("Copy")) {
					// Copy to clipboard
					std::cout << "Copied: " << logs[i].message << std::endl;
					ImGui::LogToClipboard();
					ImGui::LogText("%s", logs[i].message.c_str());
					ImGui::LogFinish();
				}
				if (ImGui::Selectable("Delete")) {
					Logger::Get().Delete(i);
				}
				if (ImGui::Selectable("Clear")) {
					Logger::Get().Clear();
				}
				ImGui::EndPopup();
			}
			ImGui::PopID();
		}
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY()) ImGui::SetScrollHereY(1.0f);

		ImGui::PopStyleVar();
		ImGui::EndChild();
		ImGui::End();
	}

	void Components::ContentBrowser() {
		ImGui::Begin("Content Browser");

		static int    buttonSize = 200;
		static ImVec2 buttonSizeVec(buttonSize, buttonSize);

		if (ImGui::SliderInt("Icon Size", &buttonSize, 20, 500)) {
			buttonSizeVec = ImVec2(buttonSize, buttonSize);
		}

		const auto& path = Application::Get().GetCurrentDirectory();

		if (std::filesystem::exists(path)) {
			ImGuiStyle& style         = ImGui::GetStyle();
			float       windowVisible = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			float       lastButtonX   = 0;

			if (ImGui::Button("..", buttonSizeVec)) {
				if (path.has_parent_path()) Application::Get().SetCurrentDirectory(path.parent_path());
			}

			for (auto& directoryEntry : std::filesystem::directory_iterator(path)) {
				ImGui::PushID(directoryEntry.path().relative_path().stem().c_str());

				if (lastButtonX + style.ItemSpacing.x + buttonSize < windowVisible) ImGui::SameLine();

				if (directoryEntry.is_directory()) {
					// if (ImGui::ImageButton(AssetManager::Get().GetTexture(
					//         "/Users/abdoulayedia/Projects/Dev/C++/"
					//         "sprite_editor/resources/icons/directory.png")))
					//         {
					// 	Application::Get().SetCurrentDirectory(
					// 	    directoryEntry.path());
					// }
					ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor(125, 125, 77, 125));
					if (ImGui::Button(directoryEntry.path().relative_path().stem().c_str(), buttonSizeVec))
						Application::Get().SetCurrentDirectory(directoryEntry.path());
					ImGui::PopStyleColor(1);
				} else if (directoryEntry.path().extension().string().find(".png") != std::string::npos) {
					if (ImGui::ImageButton(AssetManager::Get().GetTexture(directoryEntry.path().c_str()),
					                       sf::Vector2f(buttonSize, buttonSize))) {
						Application::Get().GetSpriteManager().LoadSprite(directoryEntry.path().c_str());
					}
				} else {
					ImGui::Button(directoryEntry.path().relative_path().stem().c_str(), buttonSizeVec);
				}
				lastButtonX = ImGui::GetItemRectMax().x;

				ImGui::PopID();
			}
		}

		ImGui::End();
	}

	void Components::Hierarchy() {
		ImGui::Begin("Hierarchy");

		auto& frames = Application::Get().GetSpriteManager().GetFrames();

		for (int i = 0; i < frames.size(); ++i) {
			ImGui::PushID(i);
			char label[256];
			snprintf(label, IM_ARRAYSIZE(label), "Frame %d", i);
			if (ImGui::TreeNode(label)) {
				HelpMarker("The same contents can be accessed in 'Tools->Style "
				           "Editor' or by calling "
				           "the ShowStyleEditor() function.");
				ImGui::TreePop();
				ImGui::Separator();
			}
			ImGui::PopID();
		}

		ImGui::End();
	}

	void Components::Properties() {
		ImGui::Begin("Properties");

		int currentFrameIndex = Application::Get().GetSpriteManager().GetCurrentFrameIndex();

		if (currentFrameIndex != -1) {
			sf::IntRect currentFrame = Application::Get().GetSpriteManager().GetCurrentFrame();

			if (currentFrame.width != -1) {
				sf::IntRect saveCurrentFrame = currentFrame;

				char label[256];
				snprintf(label, IM_ARRAYSIZE(label), "Frame %d", currentFrameIndex);
				ImGui::Text("%s", label);

				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode("Transform")) {
					float frameRect[4] = {static_cast<float>(currentFrame.left),
					                      static_cast<float>(currentFrame.top),
					                      static_cast<float>(currentFrame.width),
					                      static_cast<float>(currentFrame.height)};

					if (ImGui::DragFloat2("Position", frameRect, 1.f)) {
						currentFrame.left = static_cast<int>(frameRect[0]);
						currentFrame.top  = static_cast<int>(frameRect[1]);
					}

					if (ImGui::DragFloat2("Size", frameRect + 2, 1.f)) {
						currentFrame.width  = static_cast<int>(frameRect[2]);
						currentFrame.height = static_cast<int>(frameRect[3]);

						// if the width or height is changed, we need to update the position to keep the same size
						currentFrame.left += (saveCurrentFrame.width - currentFrame.width) / 2;
						currentFrame.top += (saveCurrentFrame.height - currentFrame.height) / 2;
					}

					Application::Get().GetSpriteManager().SetCurrentFrame(currentFrame);
					ImGui::TreePop();
				}

				ImGui::SetNextItemOpen(true, ImGuiCond_Once);
				if (ImGui::TreeNode("Frame")) {
					sf::Sprite s = Application::Get().GetSpriteManager().GetSprite();
					s.setTextureRect(currentFrame);
					s.setScale(2.f, 2.f);
					ImGui::Image(s);
					ImGui::TreePop();
				}

				if (ImGui::Button("Delete Frame")) {
					Application::Get().GetSpriteManager().DeleteFrame(currentFrameIndex);
				}
			}
		}

		ImGui::End();
	}

	void Components::AnimationPreview() {
		ImGui::Begin("Animation Preview");

		const auto&              frames = Application::Get().GetSpriteManager().GetFrames();
		int                      index  = Application::Get().GetSpriteManager().GetCurrentFrameIndex();
		static float             timer  = 0.f;
		static float             speed  = 1.f;
		static float             scale  = 1.5f;
		static sf::RenderTexture rt {};
		static bool              play            = false;
		static ImColor           backgroundColor = ImColor(255, 255, 255, 255);

		timer += speed * ImGui::GetIO().DeltaTime;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 10.f);
		ImGui::SliderFloat("Scale", &scale, 0.1f, 10.f);
		if (ImGui::SliderInt("Index", &index, 0, frames.size() - 1)) {
			timer = 0.f;
			play  = false;
			Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
		}
		ImGui::ColorEdit4("Background Color", (float*)&backgroundColor);

		{
			if (ImGui::Button(ICON_FA_BACKWARD)) {
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(0);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_BACKWARD_STEP)) {
				index = std::max(0, index - 1);
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
			}
			ImGui::SameLine();
			if (play) {
				if (ImGui::Button(ICON_FA_PAUSE)) {
					play = false;
				}
			} else {
				if (ImGui::Button(ICON_FA_PLAY)) {
					play = true;
				}
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_FORWARD_STEP)) {
				index = std::min(static_cast<int>(frames.size() - 1), index + 1);
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
			}
			ImGui::SameLine();
			if (ImGui::Button(ICON_FA_FORWARD)) {
				index = frames.size() - 1;
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
			}
		}

		const ImVec2 contentAvail = ImGui::GetContentRegionAvail();
		sf::Vector2f renderTextureSize(contentAvail.x, contentAvail.y);
		rt.create(static_cast<unsigned int>(renderTextureSize.x),
		          std::max(static_cast<unsigned int>(renderTextureSize.y), 1u));
		rt.clear(Maths::ImColorToSFMLColor(backgroundColor));

		if (frames.size() > 0) {
			if (timer > 0.1f && play) {
				index = (index + 1) % frames.size();
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
				timer = 0.f;
			}

			sf::Sprite s    = Application::Get().GetSpriteManager().GetSprite();
			auto       rect = frames[index];
			rect.left -= s.getPosition().x;
			rect.top -= s.getPosition().y;
			s.setTextureRect(rect);
			s.setScale(scale, scale);
			s.setOrigin(s.getTextureRect().width / 2.f, s.getTextureRect().height);
			// adjust the position y to consider the scale
			s.setPosition(renderTextureSize.x / 2.f,
			              renderTextureSize.y / 2.f - (1.f - scale) * s.getTextureRect().height / 2.f);
			// draw a grid based on the scale
			{
				sf::RectangleShape line(sf::Vector2f(renderTextureSize.x, 1.f));
				line.setFillColor(sf::Color(0, 0, 0, 50));
				for (int i = 0; i < renderTextureSize.y / scale; i++) {
					line.setPosition(0.f, i * scale * 5.f);
					rt.draw(line);
				}

				line.setSize(sf::Vector2f(1.f, renderTextureSize.y));
				for (int i = 0; i < renderTextureSize.x / scale; i++) {
					line.setPosition(i * scale * 5.f, 0.f);
					rt.draw(line);
				}

				sf::CircleShape circle(200.f);
				circle.setFillColor(sf::Color::Black);
				circle.setPosition(renderTextureSize.x / 2.f, renderTextureSize.y / 2.f);
			}
			rt.draw(s);
			ImGui::Image(rt);

			// if right click, set the current frame to the one we clicked on
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(1)) {
				Application::Get().GetSpriteManager().SetCurrentFrameIndex(index);
			}
		}

		ImGui::End();
	}

	void Components::AnimationTimeline() {
		ImGui::Begin("Animation Timeline");

		const auto& frames = Application::Get().GetSpriteManager().GetFrames();

		// ImGui::BeginGroup();
		// {
		// 	ImGui::Image();
		// }
		// ImGui::EndGroup();

		ImGui::End();
	}

	void Components::Viewport() {
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.f, 0.f));
		if (ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar)) {
			ImVec2 viewportSize                   = ImGui::GetWindowSize();
			ImVec2 viewportPos                    = ImGui::GetWindowPos();
			auto&  frames                         = Application::Get().GetSpriteManager().GetFrames();
			auto   currentFrame                   = Application::Get().GetSpriteManager().GetCurrentFrame();
			auto&  mousePos                       = Application::Get().GetWindow().GetMousePos();
			auto&  startLeftMouseButtonPressedPos = Application::Get().GetWindow().GetStartLeftMouseButtonPressedPos();
			bool   isLeftMouseButtonPressed       = Application::Get().GetWindow().GetIsLeftMouseButtonPressed();
			float  frameHeight                    = ImGui::GetFrameHeight();
			static ImVec2 gridSize(100, 100);
			sf::Sprite&   s = Application::Get().GetSpriteManager().GetSprite();

			sf::Vector2f viewPortMousePos =
			    sf::Vector2f(mousePos.x - viewportPos.x, mousePos.y - viewportPos.y - frameHeight);
			sf::IntRect        rect;
			sf::RectangleShape gridRect(sf::Vector2f(gridSize.x, gridSize.y));
			gridRect.setFillColor(sf::Color::Transparent);
			gridRect.setOutlineColor(sf::Color(111, 111, 111, 111));
			gridRect.setOutlineThickness(1.f);

			// initialize the viewport render texture
			static sf::RenderTexture rt {};
			rt.create(viewportSize.x, viewportSize.y);
			rt.clear(sf::Color(11, 11, 11));

			Application::Get().GetSpriteManager().Render(rt);
			ImGui::Image(rt);

			// Calculate the current mouse position and use it to determine the dimensions of the rectangle
			if (ImGui::IsMouseDown(0) && isLeftMouseButtonPressed && ImGui::IsWindowFocused()) {
				switch (Application::Get().CurrentTool) {
					case Tool::Slice: {
						float width  = mousePos.x - startLeftMouseButtonPressedPos.x;
						float height = mousePos.y - startLeftMouseButtonPressedPos.y;

						ImDrawList* draw_list = ImGui::GetWindowDrawList();
						rect                  = sf::IntRect(static_cast<int>(startLeftMouseButtonPressedPos.x),
                                           static_cast<int>(startLeftMouseButtonPressedPos.y),
                                           static_cast<int>(width),
                                           static_cast<int>(height));

						// if the end position is less than the start position, swap them
						if (width < 0) {
							rect.left += rect.width;
							rect.width *= -1;
						}
						if (height < 0) {
							rect.top += rect.height;
							rect.height *= -1;
						}

						draw_list->AddRect(ImVec2(rect.left, rect.top),
						                   ImVec2(rect.left + rect.width, rect.top + rect.height),
						                   IM_COL32(100, 0, 255, 255),
						                   0.0f,
						                   15,
						                   2.0f);
					}
						rect.height -= frameHeight;

						if (isLeftMouseButtonPressed && ImGui::IsWindowFocused() && rect.width > 0 && rect.height > 0 &&
						    rect.left + rect.width > 0 && rect.top + rect.height > 0) {
							rect.left -= viewportPos.x + s.getPosition().x;
							rect.top -= viewportPos.y + s.getPosition().y;
							rect.left = std::max(rect.left, 0);
							rect.top  = std::max(rect.top, 0);
							rect.width =
							    std::min(rect.width,
							             (int)Application::Get().GetSpriteManager().GetSprite().getTextureRect().width -
							                 rect.left);
							rect.height = std::min(
							    rect.height,
							    (int)Application::Get().GetSpriteManager().GetSprite().getTextureRect().height -
							        rect.top);

							if (rect.width < 0) {
								rect.left += rect.width;
								rect.width *= -1;
							}

							frames = Application::Get().GetSpriteManager().SliceSprite(rect);
							std::for_each(frames.begin(), frames.end(), [&](sf::IntRect& r) {
								r.left += startLeftMouseButtonPressedPos.x - viewportPos.x;
								r.top += startLeftMouseButtonPressedPos.y - viewportPos.y;
							});

							// sort the frames by their x position and then by their y position
							// std::sort(frames.begin(), frames.end(), [](sf::IntRect& a, sf::IntRect& b) {
							// 	if (a.left == b.left) return a.top < b.top;
							// 	return a.left < b.left;
							// });
						}
						break;

					case Tool::Select:
						break;
					case Tool::Move: {
						for (int i = 0; i < frames.size(); i++) {
							auto rect = frames[i];
							if (rect.contains(sf::Vector2i(viewPortMousePos))) {
								if (isLeftMouseButtonPressed) {
									rect.left = viewPortMousePos.x - rect.width / 2.f;
									rect.top  = viewPortMousePos.y - rect.height / 2.f;
									frames[i] = rect;
									break;
								}
							}
						}
					} break;

					case Tool::Eraser: {
						for (int i = 0; i < frames.size(); i++) {
							auto rect = frames[i];
							if (rect.contains(sf::Vector2i(viewPortMousePos))) {
								if (isLeftMouseButtonPressed) {
									frames.erase(frames.begin() + i);
									break;
								}
							}
						}
					} break;

					default:
						break;
				}
			}

			Application::Get().GetSpriteManager().SetFrames(frames);

			for (int i = 0; i < frames.size(); i++) {
				ImGui::PushID(i);
				auto      rect  = frames[i];
				sf::Color color = sf::Color::White;

				if (i == Application::Get().GetSpriteManager().GetCurrentFrameIndex())
					color = sf::Color::Blue;
				else
					color = sf::Color::Yellow;

				if (rect.contains(sf::Vector2i(viewPortMousePos))) {
					if (isLeftMouseButtonPressed) Application::Get().GetSpriteManager().SetCurrentFrameIndex(i);
					color = sf::Color::Green;
				}

				sf::RectangleShape shape;
				shape.setPosition(rect.left, rect.top);
				shape.setSize(sf::Vector2f(rect.width, rect.height));
				shape.setFillColor(sf::Color::Transparent);
				shape.setOutlineColor(color);
				shape.setOutlineThickness(1.0f);

				if (shape.getGlobalBounds().contains(viewPortMousePos)) {
					if (isLeftMouseButtonPressed) {
						Application::Get().GetSpriteManager().SetCurrentFrameIndex(i);
						shape.setFillColor(sf::Color(255, 255, 255, 100));
					} else if (ImGui::IsMouseDown(1)) {
						shape.setFillColor(sf::Color(255, 0, 0, 100));

						Application::Get().GetSpriteManager().DeleteFrame(i);
						// open the popup menu in the front
						// ImGui::OpenPopup("item context menu");
						// if (ImGui::BeginPopupContextItem("item context menu")) {
						// 	if (ImGui::MenuItem("Delete")) {
						// 		Application::Get().GetSpriteManager().DeleteFrame(i);
						// 	}
						// 	ImGui::EndPopup();
						// }
					}
				}

				// RenderDashedRectangle(rt, Application::Get().GetWindow().GetShader(), rect, color);
				rt.draw(shape);

				if (Application::Get().ShowFrameNumber) {
					sf::Text text;
					text.setString(std::to_string(i));
					text.setFont(Application::Get().GetWindow().GetFont());
					text.setCharacterSize(24);
					text.setFillColor(sf::Color::Black);
					text.setPosition(rect.left - text.getGlobalBounds().width / 2.f + rect.width / 2.f,
					                 rect.top - text.getGlobalBounds().height / 2.f + rect.height / 2.f);

					sf::CircleShape circle;
					circle.setRadius(15.0f);
					circle.setFillColor(sf::Color(255, 255, 255, 155));
					circle.setOutlineColor(sf::Color::White);
					circle.setOutlineThickness(1.0f);
					circle.setPosition(text.getPosition().x + text.getGlobalBounds().width / 2.f - circle.getRadius(),
					                   text.getPosition().y + text.getGlobalBounds().height - circle.getRadius());
					rt.draw(circle);
					rt.draw(text);
				}

				ImGui::PopID();
			}

			if (Application::Get().GetWindow().GetShowGrid()) {
				for (uint16_t i = 0; i < viewportSize.y / gridSize.y; ++i) {
					for (uint16_t j = 0; j < viewportSize.x / gridSize.x; ++j) {
						gridRect.setPosition(j * gridSize.x, i * gridSize.y);
						rt.draw(gridRect);
					}
				}
			}

			// I want to make a menu like on Blender, it is on the left side of the viewport, vertical buttons that you
			// can click on to select a tool (like move, scale, rotate, etc)
			// Set the position of the menu to be on the left side of the viewport
			ImGui::SetCursorPos(ImVec2(viewportPos.x, viewportPos.y + 50));
			ToolBox();
		}
		ImGui::End();
	}

	void Components::Frames() {
		ImGui::Begin("Frames");
		{
			static int selected                  = -1;
			ImVec2     viewportSize              = ImGui::GetWindowSize();
			ImVec2     viewportPos               = ImGui::GetWindowPos();
			auto&      frames                    = Application::Get().GetSpriteManager().GetFrames();
			auto&      mousePos                  = Application::Get().GetWindow().GetMousePos();
			auto& startLeftMouseButtonPressedPos = Application::Get().GetWindow().GetStartLeftMouseButtonPressedPos();
			bool  isLeftMouseButtonPressed       = Application::Get().GetWindow().GetIsLeftMouseButtonPressed();

			for (int i = 0; i < frames.size(); i++) {
				ImGui::PushID(i);
				char label[128];
				snprintf(label, IM_ARRAYSIZE(label), "Frame %d", i);

				sf::Sprite sprite = Application::Get().GetSpriteManager().GetSprite();
				sprite.setPosition(0, 0);
				sprite.setTexture(AssetManager::Get().GetTexture(__DEFAULT_SPRITE__));
				sprite.setTextureRect(sf::IntRect(frames[i].left,  // x
				                                  frames[i].top,   // y
				                                  frames[i].width, // width
				                                  frames[i].height // height
				                                  ));

				sf::Vector2f size;
				// the size should make the sprite fit in the image button 100x100 without being distorted
				if (sprite.getTextureRect().width > sprite.getTextureRect().height) {
					size.x = 100;
					size.y = sprite.getTextureRect().height * (100.f / sprite.getTextureRect().width);
				} else {
					size.y = 100;
					size.x = sprite.getTextureRect().width * (100.f / sprite.getTextureRect().height);
				}

				if (ImGui::ImageButton(sprite, size)) {
					Application::Get().GetSpriteManager().SetCurrentFrameIndex(i);
				}

				ImGui::SameLine();
				ImGui::Text("%s", label);
				ImGui::PopID();
			}
		}
		ImGui::End();
	}

	void Components::ShaderEditor() {
		ImGui::Begin("Shader Editor");

		std::string shaderCode = R"(
		#ifdef GL_ES
		precision mediump float;
		#endif

		uniform vec2  u_resolution;
		uniform vec2  u_rectPos;     // position of the rectangle
		uniform vec2  u_rectSize;    // size of the rectangle
		uniform float u_borderWidth; // width of the border in screen space
		uniform float u_dashLength;  // length of each dash in the border

		float rectShape(vec2 st, vec2 center, vec2 size) {
			vec2 d = abs(st - center) - size;
			return vec2(step(d.x, 0.0), step(d.y, 0.0)).x * vec2(step(d.x, 0.0), step(d.y, 0.0)).y;
		}

		float border(vec2 st, vec2 center, vec2 size, float borderWidth, float dashLength) {
			vec2  d    = abs(st - center) - size;
			float dist = length(max(d, vec2(0.0)));
			float dash = mod(dist, dashLength * 2.0);
			return step(borderWidth, dist) * step(dashLength, dash);
		}

		void main() {
			vec2 st    = gl_FragCoord.xy / u_resolution;
			vec4 color = vec4(0.0);

			// draw the rectangle
			float rect = rectShape(st, u_rectPos, u_rectSize);
			color      = mix(color, vec4(1.0), rect);

			// draw the border
			float border = border(st, u_rectPos, u_rectSize, u_borderWidth, u_dashLength);
			color        = mix(color, vec4(1.0), border);

			gl_FragColor = color;
		}
		)";

		static char shaderText[4096];
		std::strcpy(shaderText, shaderCode.c_str());

		if (ImGui::InputTextMultiline("##Shader",
		                              shaderText,
		                              IM_ARRAYSIZE(shaderText),
		                              // take the whole window
		                              ImVec2(-FLT_MIN, -FLT_MIN),
		                              ImGuiInputTextFlags_AllowTabInput)) {
			Application::Get().GetWindow().GetShader().loadFromMemory(shaderText, sf::Shader::Fragment);
		}

		ImGui::End();
	}

	void Components::ShaderProperties() {
		ImGui::Begin("Shader Properties");
		sf::Shader& shader = Application::Get().GetWindow().GetShader();
		if (!shader.isAvailable()) {
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Shader is not available");
			ImGui::End();
			return;
		}

		static float   lineWidth  = 0.01f;
		static float   dashLength = 0.01f;
		static ImColor color      = ImColor(1.0f, 1.0f, 1.0f, 1.0f);

		ImGui::DragFloat("Border Width", &lineWidth, 0.001f, 0.01f, 1.0f);
		ImGui::DragFloat("Dash Length", &dashLength, 0.001f, 0.01f, 1.0f);
		ImGui::ColorEdit4("Color", (float*)&color);

		ImGui::End();

		shader.setUniform("u_borderWidth", lineWidth);
		shader.setUniform("u_dashLength", dashLength);
		shader.setUniform("u_color", sf::Glsl::Vec4(color.Value.x, color.Value.y, color.Value.z, color.Value.w));
	}

	void Components::OpenCVProperties() {
		ImGui::Begin("OpenCV Properties");

		// create appropriate input for this:
		// cv::Size  MorphologicalKernelSize = {3, 3};
		// cv::Point MorphologicalAnchor     = {-1, -1};
		// int       MorphologicalIterations = 1;
		// int       BoundingRectAreaMin     = 50;
		// double    TreeThresholdMin        = 0.0;
		// double    TreeThresholdMax        = 255.0;
		// Gaussian Blur size, must be odd
		static int gaussianBlurSize[2]        = {Application::Get().GaussianBlurSize.width,
		                                         Application::Get().GaussianBlurSize.height};
		static int morphologicalKernelSize[2] = {Application::Get().MorphologicalKernelSize.width,
		                                         Application::Get().MorphologicalKernelSize.height};
		static int morphologicalAnchor[2]     = {Application::Get().MorphologicalAnchor.x,
		                                         Application::Get().MorphologicalAnchor.y};
		static int morphologicalIterations    = Application::Get().MorphologicalIterations;
		static int boundingRectAreaMin        = Application::Get().BoundingRectAreaMin;
		static int treeThreshold[2] = {Application::Get().TreeThresholdMin, Application::Get().TreeThresholdMax};

		if (ImGui::DragInt2("Gaussian Blur Size", (int*)&gaussianBlurSize, 2, 1, 100)) {
			Application::Get().GaussianBlurSize.width  = gaussianBlurSize[0];
			Application::Get().GaussianBlurSize.height = gaussianBlurSize[1];
		}

		if (ImGui::DragInt2("Morphological Kernel Size", (int*)&morphologicalKernelSize, 2, 1, 100)) {
			Application::Get().MorphologicalKernelSize.width  = morphologicalKernelSize[0];
			Application::Get().MorphologicalKernelSize.height = morphologicalKernelSize[1];
		}

		if (ImGui::DragInt2("Morphological Anchor", (int*)&morphologicalAnchor, 2, -100, 100)) {
			Application::Get().MorphologicalAnchor.x = morphologicalAnchor[0];
			Application::Get().MorphologicalAnchor.y = morphologicalAnchor[1];
		}

		if (ImGui::DragInt("Morphological Iterations", &morphologicalIterations, 1, 1, 100)) {
			Application::Get().MorphologicalIterations = morphologicalIterations;
		}

		if (ImGui::DragInt("Bounding Rect Area Min", &boundingRectAreaMin, 1, 1, 10000)) {
			Application::Get().BoundingRectAreaMin = boundingRectAreaMin;
		}

		if (ImGui::DragInt2("Tree Threshold", (int*)&treeThreshold, 1, 0, 255)) {
			Application::Get().TreeThresholdMin = treeThreshold[0];
			Application::Get().TreeThresholdMax = treeThreshold[1];
		}

		ImGui::End();
	}

	void Components::ToolBox() {
		// ImGui::BeginChild("ViewportMenu",
		//                   ImVec2(0, 0),
		//                   false,
		//                   ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.25f, 0.25f, 0.25f, 0.65f));
		{
			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Select", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Select;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Slice", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Slice;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Move", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Move;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Rotate", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Rotate;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Scale", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Scale;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Zoom", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Zoom;
			}

			ImGui::SetCursorPosX(15);
			if (ImGui::Button("Delete", ImVec2(100, 100))) {
				Application::Get().CurrentTool = Tool::Eraser;
			}

			ImGui::SetCursorPosX(15);
			// checkboxes
			ImGui::Checkbox("Show Frame Number", &Application::Get().ShowFrameNumber);
		}
		ImGui::PopStyleColor();
		// ImGui::EndChild();
	}

	void Components::RenderDashedRectangle(sf::RenderTarget&  target,
	                                       sf::Shader&        shader,
	                                       const sf::IntRect& rect,
	                                       const sf::Color&   color) {
		sf::VertexArray lines[4];
		for (int i = 0; i < 4; i++) {
			lines[i].setPrimitiveType(sf::Lines);
			lines[i].resize(2);
			for (int j = 0; j < 2; j++) {
				lines[i][j].color = color;
			}
		}

		lines[0][0].position = sf::Vector2f(rect.left, rect.top);
		lines[0][1].position = sf::Vector2f(rect.left, rect.top + rect.height);

		lines[1][0].position = sf::Vector2f(rect.left, rect.top + rect.height);
		lines[1][1].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);

		lines[2][0].position = sf::Vector2f(rect.left + rect.width, rect.top + rect.height);
		lines[2][1].position = sf::Vector2f(rect.left + rect.width, rect.top);

		lines[3][0].position = sf::Vector2f(rect.left + rect.width, rect.top);
		lines[3][1].position = sf::Vector2f(rect.left, rect.top);

		Application::Get().GetWindow().GetShader().setUniform("u_color",
		                                                      sf::Glsl::Vec4(color.r, color.g, color.b, color.a));

		for (int i = 0; i < 4; i++) {
			target.draw(lines[i], &shader);
		}
	}

	void Components::HelpMarker(const char* desc) {
		ImGui::TextDisabled("(?)");
		if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort)) {
			ImGui::BeginTooltip();
			ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
			ImGui::TextUnformatted(desc);
			ImGui::PopTextWrapPos();
			ImGui::EndTooltip();
		}
	}

} // namespace se
