//
// Created by abdoulayedia on 25.12.2022
//
#pragma once

#include "imgui-SFML.h"
#include "imgui.h"
#include "imgui_internal.h"

#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <algorithm>
#include <chrono>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <map>
#include <sstream>
#include <stdio.h>
#include <string>
#include <unordered_map>
#include <vector>

namespace se {

	enum class LogLevel { Info, Warning, Error, Fatal };

	struct Log {
		Log() = default;
		Log(const Log& log) {
			level   = log.level;
			message = log.message;
			file    = log.file;
			line    = log.line;
			time    = log.time;
		}
		Log(LogLevel           level,
		    const std::string& message,
		    const std::string& file = "",
		    int                line = 0,
		    std::time_t        time = std::time(nullptr))
		    : level(level), message(message), file(file), line(line), time(time) {}

		~Log() = default;

		LogLevel    level;
		std::string message;
		std::string file;
		int         line;
		std::time_t time;
	};

	// Logger
	class Logger {
	  public:
		static Logger& Get() {
			static Logger instance;
			return instance;
		}

		void Info(const std::string& message, const std::string& file = "", int line = 0);
		void Warn(const std::string& message, const std::string& file = "", int line = 0);
		void Error(const std::string& message, const std::string& file = "", int line = 0);
		void Fatal(const std::string& message, const std::string& file = "", int line = 0);

		void Delete(int index);
		void Clear();

		std::vector<Log>& GetLogs();

		static ImColor GetColor(LogLevel level);

	  private:
		Logger();
		~Logger();

		std::vector<Log> _logs;

	  public:
		Logger(Logger const&)            = delete;
		Logger& operator=(const Logger&) = delete;
	};
} // namespace se
