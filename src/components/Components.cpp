#include "Components.h"

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
					}
					if (ImGui::MenuItem("Open", "Ctrl+O")) {
					}
					if (ImGui::MenuItem("Save", "Ctrl+S")) {
					}
					if (ImGui::MenuItem("Save As...")) {
					}
					ImGui::Separator();
					if (ImGui::MenuItem("Exit")) {
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
				ImGui::Text("Main menu bar, file, edit, etc.");

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