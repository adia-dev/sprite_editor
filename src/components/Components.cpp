#include "Components.h"

#include "application/Application.h"

namespace se {

	void Components::MenuBar(uint16_t windowWidth) {
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
				ImGui::EndMenuBar();
			}
			ImGui::End();
		}

		if (ImGui::BeginViewportSideBar("##SecondaryMenuBar", viewport, ImGuiDir_Up, height, window_flags)) {
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
		static int               index  = 0;
		static float             timer  = 0.f;
		static float             speed  = 1.f;
		static float             scale  = 1.f;
		static sf::RenderTexture rt {};
		static ImColor           backgroundColor = ImColor(255, 255, 255, 255);

		timer += speed * ImGui::GetIO().DeltaTime;

		ImGui::SliderFloat("Speed", &speed, 0.1f, 10.f);
		ImGui::SliderFloat("Scale", &scale, 0.1f, 10.f);
		ImGui::SliderInt("Index", &index, 0, frames.size() - 1);
		ImGui::ColorEdit4("Background Color", (float*)&backgroundColor);

		const ImVec2 contentAvail = ImGui::GetContentRegionAvail();
		sf::Vector2f renderTextureSize(contentAvail.x, contentAvail.y);
		rt.create(static_cast<unsigned int>(renderTextureSize.x), static_cast<unsigned int>(renderTextureSize.y));
		rt.clear(Maths::ImColorToSFMLColor(backgroundColor));

		if (frames.size() > 0) {
			if (timer > 0.1f) {
				index = (index + 1) % frames.size();
				timer = 0.f;
			}

			sf::Sprite s = Application::Get().GetSpriteManager().GetSprite();
			s.setTextureRect(frames[index]);
			s.setScale(scale, scale);
			s.setOrigin(s.getTextureRect().width / 2.f, s.getTextureRect().height / 2.f);
			s.setPosition(renderTextureSize.x / 2.f, renderTextureSize.y / 2.f);
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
		if (ImGui::Begin("Viewport")) {
			ImVec2 viewportSize                   = ImGui::GetWindowSize();
			ImVec2 viewportPos                    = ImGui::GetWindowPos();
			auto&  frames                         = Application::Get().GetSpriteManager().GetFrames();
			auto   currentFrame                   = Application::Get().GetSpriteManager().GetCurrentFrame();
			auto&  mousePos                       = Application::Get().GetWindow().GetMousePos();
			auto&  startLeftMouseButtonPressedPos = Application::Get().GetWindow().GetStartLeftMouseButtonPressedPos();
			bool   isLeftMouseButtonPressed       = Application::Get().GetWindow().GetIsLeftMouseButtonPressed();
			float  frameHeight                    = ImGui::GetFrameHeight();
			sf::Vector2f viewPortMousePos =
			    sf::Vector2f(mousePos.x - viewportPos.x, mousePos.y - viewportPos.y - frameHeight);

			// initialize the viewport render texture
			sf::IntRect              rect;
			static sf::RenderTexture rt {};
			rt.create(viewportSize.x, viewportSize.y);
			rt.clear(sf::Color(11, 11, 11));

			Application::Get().GetSpriteManager().Render(rt);
			ImGui::Image(rt);

			// Calculate the current mouse position and use it to determine the dimensions of the rectangle
			if (ImGui::IsMouseDown(0) && isLeftMouseButtonPressed && ImGui::IsWindowFocused()) {
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
				rect.left -= viewportPos.x;
				rect.top -= viewportPos.y;
				rect.left = std::max(rect.left, 0);
				rect.top  = std::max(rect.top, 0);
				rect.width =
				    std::min(rect.width,
				             (int)Application::Get().GetSpriteManager().GetSprite().getTextureRect().width - rect.left);
				rect.height =
				    std::min(rect.height,
				             (int)Application::Get().GetSpriteManager().GetSprite().getTextureRect().height - rect.top);

				Logger::Get().Clear();
				Logger::Get().Info("start: " + std::to_string(rect.left) + ", " + std::to_string(rect.top) + ", " +
				                   std::to_string(rect.width) + ", " + std::to_string(rect.height));
				Logger::Get().Info("end: " + std::to_string(rect.left + rect.width) + ", " +
				                   std::to_string(rect.top + rect.height));

				// if the rectangle is reversed,offset the start position
				if (rect.width < 0) {
					rect.left += rect.width;
					rect.width *= -1;
				}

				frames = Application::Get().GetSpriteManager().SliceSprite(rect);
				std::for_each(frames.begin(), frames.end(), [&](sf::IntRect& r) {
					r.left += startLeftMouseButtonPressedPos.x - viewportPos.x;
					r.top += startLeftMouseButtonPressedPos.y - viewportPos.y;
				});
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
				ImGui::PopID();
			}
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
