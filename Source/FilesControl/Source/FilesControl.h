#pragma once

#ifdef __EXPORT_FILES_CONTROL_DLL
#define FILESCONTROL_API __declspec(dllexport)
#else
#define FILESCONTROL_API __declspec(dllimport)
#endif

#include <filesystem>
#include <fstream>

namespace fs = std::filesystem;

typedef std::fstream File;

class FILESCONTROL_API FilesControl
{
public:
	File GetFile(std::string_view path);
	File CreateFile(std::string_view path, std::ios_base::openmode mode = std::ios::out);
	inline bool DoesFileExist(std::string_view path);
	inline bool DoesFolderExist(std::string_view path);
	bool CreateFolder(std::string_view path);
};

class FILESCONTROL_API TxtParser
{
public:
	bool GetValue(std::string_view filePath, std::string_view key, std::string& result);
};

extern FILESCONTROL_API FilesControl* const SFilesControl;
extern FILESCONTROL_API TxtParser* const STxtParser;