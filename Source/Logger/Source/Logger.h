#pragma once

#include <string_view>
#include <fstream>

#ifdef __EXPORT_LOGGER_DLL
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif


enum class EMessageType
{
	kOk,
	kError,
	kWarning
};

class Logger
{
public:
	virtual ~Logger() noexcept = default;
	Logger& operator=(const Logger&) = delete;
	Logger&& operator=(const Logger&&) = delete;
	Logger* operator=(const Logger*) = delete;
	Logger** operator=(const Logger**) = delete;
	Logger(const Logger&) = delete;
	Logger(const Logger&&) noexcept = delete;

public:
	static Logger* Instance();
	void Print(std::string_view message);
	void Print(std::string_view message, EMessageType type);
private:
	explicit Logger() noexcept;

private:
	std::fstream LoggerFile_;
};

extern Logger* const SLog;