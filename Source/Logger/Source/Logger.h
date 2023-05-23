#pragma once

#include <string_view>

#ifdef __EXPORT_DLL
#define LOGGER_API __declspec(dllexport)
#else
#define LOGGER_API __declspec(dllimport)
#endif


class LOGGER_API Logger
{
public:
	virtual ~Logger() noexcept = default;
	Logger& operator=(const Logger&) = delete;
	Logger&& operator=(const Logger&&) = delete;
	Logger(const Logger&) = delete;
	Logger(const Logger&&) noexcept = delete;

public:
	static Logger* Instance();
	void Print(std::string_view message);
private:
	explicit Logger() noexcept = default;
};

extern LOGGER_API Logger* const SLog;