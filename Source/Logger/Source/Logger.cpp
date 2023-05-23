#include "Logger.h"

#include <iostream>

Logger* Logger::Instance()
{
	static Logger lg;
	return &lg;
}

void Logger::Print(std::string_view message)
{
	std::cout << message << '\n';
}

Logger* const SLog = Logger::Instance();