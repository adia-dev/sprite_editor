#include "Components.h"

#include "application/Application.h"

namespace se {

	void Components::MenuBar(uint16_t windowWidth) {
		ImGuiViewportP* viewport =
		    (ImGuiViewportP*)(void*)ImGui::GetMainViewport();

		ImGuiWindowFlags window_flags =
		    ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
		    ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar;

		float height = ImGui::GetFrameHeight();

		// Main menu bar
		if (ImGui::BeginViewportSideBar("##MainMenuBar",
		                                viewport,
		                                ImGuiDir_Up,
		                                height,
		                                window_flags)) {
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
					if (ImGui::MenuItem("Redo",
					                    "Ctrl+Y",
					                    false,
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
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

		if (ImGui::BeginViewportSideBar("##SecondaryMenuBar",
		                                viewport,
		                                ImGuiDir_Up,
		                                height,
		                                window_flags)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::Button("Dev");
				ImGui::SameLine();
				ImGui::Button("Game");
				ImGui::SameLine();
				ImGui::Button("Scene");

				ImGui::EndMenuBar();
			}

			ImGui::End();
		}

		if (ImGui::BeginViewportSideBar("##MainStatusBar",
		                                viewport,
		                                ImGuiDir_Down,
		                                height,
		                                window_flags)) {
			if (ImGui::BeginMenuBar()) {
				ImGui::Text("Status bar, project name, unsaved changes, etc.");
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}
	}

	void Components::Console(const std::vector<std::string>& logs) {
		ImGui::Begin("Console");
		static char searchBuffer[256] = {0};

		// Inline buttons and a search bar
		{
			ImGui::BeginChild("Buttons", ImVec2(0, 50));
			ImGui::SameLine();
			if (ImGui::Button("Clear")) {
				// Clear the log
				std::cout << "Clearing the log" << std::endl;
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
			                         ImGuiInputTextFlags_AutoSelectAll |
			                             ImGuiInputTextFlags_EnterReturnsTrue);
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
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
		                    ImVec2(4, 1)); // Tighten spacing
		for (int i = 0; i < logs.size(); i++) {
			// Clickable text, with action such has right click opens a menu
			// with copy, etc.

			// filter logs if searchBuffer is not empty
			if (searchBuffer[0] != '\0') {
				if (logs[i].find(searchBuffer) == std::string::npos) {
					continue;
				}
			}

			if (ImGui::Selectable(logs[i].c_str(),
			                      false,
			                      ImGuiSelectableFlags_AllowDoubleClick)) {
				if (ImGui::IsMouseDoubleClicked(0)) {
					// Do something on double click
					std::cout << "Double clicked on: " << logs[i] << std::endl;
				}
			}

			// Right click on text
			if (ImGui::BeginPopupContextItem()) {
				if (ImGui::Selectable("Copy")) {
					// Copy to clipboard
					std::cout << "Copied: " << logs[i] << std::endl;
				}
				if (ImGui::Selectable("Delete")) {
					// Delete from logs
					std::cout << "Deleted: " << logs[i] << std::endl;
				}
				if (ImGui::Selectable("Clear")) {
					// Clear the log
					std::cout << "Clearing the log" << std::endl;
				}
				ImGui::EndPopup();
			}
		}
		if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
			ImGui::SetScrollHereY(1.0f);

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
			ImGuiStyle& style = ImGui::GetStyle();
			float       windowVisible =
			    ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;
			float lastButtonX = 0;

			if (ImGui::Button("..", buttonSizeVec)) {
				if (path.has_parent_path())
					Application::Get().SetCurrentDirectory(path.parent_path());
			}

			for (auto& directoryEntry :
			     std::filesystem::directory_iterator(path)) {
				ImGui::PushID(
				    directoryEntry.path().relative_path().stem().c_str());

				if (lastButtonX + style.ItemSpacing.x + buttonSize <
				    windowVisible)
					ImGui::SameLine();

				if (directoryEntry.is_directory()) {
					// if (ImGui::ImageButton(AssetManager::Get().GetTexture(
					//         "/Users/abdoulayedia/Projects/Dev/C++/"
					//         "sprite_editor/resources/icons/directory.png")))
					//         {
					// 	Application::Get().SetCurrentDirectory(
					// 	    directoryEntry.path());
					// }
					ImGui::PushStyleColor(ImGuiCol_Button,
					                      (ImVec4)ImColor(125, 125, 77, 125));
					if (ImGui::Button(directoryEntry.path()
					                      .relative_path()
					                      .stem()
					                      .c_str(),
					                  buttonSizeVec))
						Application::Get().SetCurrentDirectory(
						    directoryEntry.path());
					ImGui::PopStyleColor(1);
				} else if (directoryEntry.path().extension().string().find(
				               ".png") != std::string::npos) {
					if (ImGui::ImageButton(
					        AssetManager::Get().GetTexture(
					            directoryEntry.path().c_str()),
					        sf::Vector2f(buttonSize, buttonSize))) {
						Application::Get().GetSpriteManager().LoadSprite(
						    directoryEntry.path().c_str());
					}
				} else {
					ImGui::Button(
					    directoryEntry.path().relative_path().stem().c_str(),
					    buttonSizeVec);
				}
				lastButtonX = ImGui::GetItemRectMax().x;

				ImGui::PopID();
			}
		}

		ImGui::End();
	}

	void Components::Hierarchy() {
		ImGui::Begin("Hierarchy");

		if (ImGui::TreeNode("Circle1")) {
			HelpMarker("The same contents can be accessed in 'Tools->Style "
			           "Editor' or by calling "
			           "the ShowStyleEditor() function.");
			ImGui::ShowStyleEditor();
			ImGui::TreePop();
			ImGui::Separator();
		}

		ImGui::End();
	}

	void Components::Properties() {
		ImGui::Begin("Properties");

		static ImVec2      position {0, 0};
		static float       scale = 1.f;
		static sf::Sprite& sprite =
		    Application::Get().GetSpriteManager().GetSprite();

		if (ImGui::TreeNode("Goku")) {
			HelpMarker("The same contents can be accessed in 'Tools->Style "
			           "Editor' or by calling "
			           "the ShowStyleEditor() function.");

			if (ImGui::SliderFloat2("Position",
			                        (float*)&position,
			                        -1000,
			                        1000)) {
				sprite.setPosition(position.x, position.y);
			}

			if (ImGui::SliderFloat("Scale", &scale, 1, 10)) {
				sprite.setScale(scale, scale);
			}

			ImGui::TreePop();
			ImGui::Separator();
		}

		ImGui::End();
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
