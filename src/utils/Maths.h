#pragma once

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace se {

	class Maths {
	  public:
		Maths(/* args */) {}
		~Maths() {}

		static sf::Color ImVec4toSFColor(ImVec4 color) {
			return sf::Color(color.x * 255,
			                 color.y * 255,
			                 color.z * 255,
			                 color.w * 255);
		}

	  private:
		/* data */
	};
} // namespace se