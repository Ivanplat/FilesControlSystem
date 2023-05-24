#include "Logger.h"

#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::Print(std::string_view message)
{
	std::cout << message << '\n';
}

void Logger::Print(std::string_view message, EMessageType type)
{
	std::string prefix = "";
	std::string resultString = "";
	switch (type)
	{
	case EMessageType::kOk: prefix = "LOG: "; break;
	case EMessageType::kError: prefix = "ERROR: "; break;
	case EMessageType::kWarning: prefix = "WARNING: "; break;
	}

}

Logger::Logger() noexcept
{
	std::string loggerPath = fs::current_path().string() + "\\log";
	if (!fs::is_directory(loggerPath))
	{
		if (!fs::create_directory(loggerPath))
		{
			throw std::exception("Could not create the logger directory!");
		}
	}
	std::string filePath = loggerPath + "\\log.txt";
	LoggerFile_.open(filePath);
	if (!LoggerFile_.is_open())
	{
		LoggerFile_.clear();
		LoggerFile_.close();
		LoggerFile_.open(filePath, std::ios::out);
		if (!LoggerFile_.is_open())
		{
			throw std::exception("Could not create the logger file!");
		}
		LoggerFile_ << " ";
	}
}

Logger* const SLog = Logger::Instance();