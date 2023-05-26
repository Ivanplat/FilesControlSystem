#pragma once

#ifdef __EXPORT_FILES_CONTROL_DLL
#define FILESCONTROL_API __declspec(dllexport)
#else
#define FILESCONTROL_API __declspec(dllimport)
#endif

#include <filesystem>
#include <fstream>
#include <memory>
#include <map>

namespace fs = std::filesystem;

typedef std::fstream File;

enum
{
	kConfigFilesCreated = 0,
	kConfigFilesExist = 1,
	kCouldNotCreateConfigFiles = -1
};

struct FILESCONTROL_API AppSettings
{
	std::uint16_t WindowHeight;
	std::uint16_t WindowWidth;
};

struct FILESCONTROL_API ClientServerSettings : public AppSettings
{
	std::string Port;
};

struct FILESCONTROL_API ClientSettings : public ClientServerSettings
{
	std::string Adress;
};

struct FILESCONTROL_API ServerSettings : public ClientServerSettings
{
};

template<class T = AppSettings>
class FILESCONTROL_API SettingsManager
{
public:
	explicit SettingsManager();
	
public:
	virtual std::int16_t CreateSettingsFiles();
	virtual void ParseSettings(std::map<std::string_view, std::string>& values) {}

public:
	T Settings;
	std::string SettingsPath;

};

class FILESCONTROL_API ClientSettingsManager final : public SettingsManager<ClientSettings>
{
public:
	virtual std::int16_t CreateSettingsFiles() override;
	virtual void ParseSettings(std::map<std::string_view, std::string>& values) override;
	File GetConnectionConfig();
};

class FILESCONTROL_API ServerSettingsManager final : public SettingsManager<ServerSettings>
{
public:
	virtual std::int16_t CreateSettingsFiles() override;
	virtual void ParseSettings(std::map<std::string_view, std::string>& values) override;
	File GetServerConfig();
};

class FILESCONTROL_API SettingManagerFactory
{
public:
	template<class T, class AppSettingsType>
	std::unique_ptr<T> ConstructSettingsManager()
	{
		T object = T();
		if (dynamic_cast<SettingsManager<AppSettingsType>*>(&object))
		{
			return std::make_unique<T>(std::move(object));
		}
		return nullptr;
	}
};


class FILESCONTROL_API FilesControl
{
public:
	File GetFile(std::string_view path);
	File CreateFileByPath(std::string_view path, std::ios_base::openmode mode = std::ios::out);
	inline fs::path GetCurrentPath() const;
	inline bool DoesFileExist(std::string_view path);
	inline bool DoesFolderExist(std::string_view path);
	bool CreateFolder(std::string_view path);
	bool GetFileInfo(std::string_view path, std::string& out);
};

class FILESCONTROL_API TxtParser
{
public:
	bool GetValue(std::string_view filePath, std::string_view key, std::string& result);
};

void FILESCONTROL_API remove_spaces(std::string&& str);

extern FILESCONTROL_API FilesControl* const SFilesControl;
extern FILESCONTROL_API TxtParser* const STxtParser;