//
// Created by abdoulayedia on 01.01.2023
//
#include "Serializer.h"

#include "application/Application.h"

#include <tinyxml2.h>

namespace se {
	void Serializer::SaveFramesToXML(const std::filesystem::path& path, const std::vector<sf::IntRect>& frames) {
		if (!std::filesystem::exists(path.parent_path())) {
			std::filesystem::create_directories(path.parent_path());
		}

		if (!path.has_extension()) {
			Logger::Get().Error("[Serializer] The path must have an extension");
			return;
		}

		// Create XML document
		tinyxml2::XMLDocument doc;

		// Create the root element
		tinyxml2::XMLElement* root      = doc.NewElement("Animations");
		tinyxml2::XMLElement* animation = doc.NewElement("Animation");
		animation->SetAttribute("count", (int)frames.size());
		animation->SetAttribute("name", Application::Get().GetSpriteManager().AnimationName);
		doc.InsertFirstChild(root);

		// Add the frames to the root element
		for (const sf::IntRect& frame : frames) {
			tinyxml2::XMLElement* element = doc.NewElement("Frame");
			element->SetAttribute("x", frame.left);
			element->SetAttribute("y", frame.top);
			element->SetAttribute("width", frame.width);
			element->SetAttribute("height", frame.height);
			animation->InsertEndChild(element);
		}

		root->InsertEndChild(animation);

		// Save the XML document to a file
		doc.SaveFile(path.string().c_str());
	}
	std::vector<sf::IntRect> Serializer::LoadFramesFromXML(const std::filesystem::path& path) {
		std::vector<sf::IntRect> frames;

		if (!std::filesystem::exists(path)) {
			Logger::Get().Error("[Serializer] The path does not exist");
			return frames;
		}

		if (!path.has_extension()) {
			Logger::Get().Error("[Serializer] The path must have an extension");
			return frames;
		}

		// Create XML document
		tinyxml2::XMLDocument doc;

		// Load the XML document from a file
		doc.LoadFile(path.string().c_str());

		// Get the root element
		tinyxml2::XMLElement* root = doc.FirstChildElement("Animations");
		if (root == nullptr) {
			Logger::Get().Error("[Serializer] The root element is not 'Animations'");
			return frames;
		}

		for (tinyxml2::XMLElement* element = root->FirstChildElement("Animation"); element != nullptr;
		     element                       = element->NextSiblingElement("Animation")) {
			int count = 0;
			element->QueryIntAttribute("count", &count);
			if (count == 0) {
				Logger::Get().Error("[Serializer] The attribute 'count' is missing");
				return frames;
			}

			char* name = (char*)element->Attribute("name");
			if (name == nullptr) {
				Logger::Get().Error("[Serializer] The attribute 'name' is missing");
				return frames;
			}
			snprintf(Application::Get().GetSpriteManager().AnimationName, strlen(name) + 1, "%s", name);

			// Get the frames from the root element
			for (tinyxml2::XMLElement* frame = element->FirstChildElement("Frame"); frame != nullptr;
			     frame                       = frame->NextSiblingElement("Frame")) {
				int x, y, width, height;
				frame->QueryIntAttribute("x", &x);
				frame->QueryIntAttribute("y", &y);
				frame->QueryIntAttribute("width", &width);
				frame->QueryIntAttribute("height", &height);
				frames.emplace_back(x, y, width, height);
			}
		}

		return frames;
	}
} // namespace se
