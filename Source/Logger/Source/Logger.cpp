#include "Logger.h"

#include <iostream>
#include <filesystem>
#include <format>
#include "FilesControl.h"

namespace fs = std::filesystem;

Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::Print(std::string_view message)
{
	std::cout << message << '\n';
	LoggerFile_ << message << "\n";
}

void Logger::Print(std::string_view message, EMessageType type)
{
	std::string prefix = "";
	switch (type)
	{
	case EMessageType::kOk: prefix = "LOG: "; break;
	case EMessageType::kError: prefix = "ERROR: "; break;
	case EMessageType::kWarning: prefix = "WARNING: "; break;
	}
	Print(std::format("{}{}", prefix, message));
}

Logger::Logger() noexcept
{
	std::string loggerPath = fs::current_path().string() + "\\log";
	if (!SFilesControl->DoesFolderExist(loggerPath))
	{
		if (!SFilesControl->CreateFolder(loggerPath))
		{
			throw std::exception("Could not create the logger directory!");
		}
	}
	std::string filePath = loggerPath + "\\log.txt";
	if (!SFilesControl->DoesFileExist(filePath))
	{
		LoggerFile_ = SFilesControl->CreateFile(filePath, std::ios::app);
	}
}

Logger* const SLog = Logger::Instance();