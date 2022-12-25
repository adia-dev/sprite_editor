#include "Logger.h"

#include "application/Application.h"

namespace se {

	Logger::Logger() {
		_logs.reserve(100);
	}

	Logger::~Logger() {
		_logs.clear();
	}

	void Logger::Info(const std::string& message, const std::string& file, int line) {
		_logs.push_back(Log(LogLevel::Info, message, file, line, Application::Get().Now()));
	}

	void Logger::Warn(const std::string& message, const std::string& file, int line) {
		_logs.push_back(Log(LogLevel::Warning, message, file, line, Application::Get().Now()));
	}

	void Logger::Error(const std::string& message, const std::string& file, int line) {
		_logs.push_back(Log(LogLevel::Error, message, file, line, Application::Get().Now()));
	}

	void Logger::Fatal(const std::string& message, const std::string& file, int line) {
		_logs.push_back(Log(LogLevel::Fatal, message, file, line, Application::Get().Now()));
	}
	std::vector<Log>& Logger::GetLogs() {
		return _logs;
	}

	ImColor Logger::GetColor(LogLevel level) {
		switch (level) {
			case LogLevel::Info:
				return ImColor(200, 200, 200, 255);
			case LogLevel::Warning:
				return ImColor(255, 255, 128, 255);
			case LogLevel::Error:
				return ImColor(255, 128, 128, 255);
			case LogLevel::Fatal:
				return ImColor(255, 128, 128, 255);
			default:
				return ImColor(128, 128, 128, 255);
		}
	}

} // namespace se