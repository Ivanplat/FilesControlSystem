#include "FilesControl.h"
#include "Logger.h"

#include <algorithm>

static FilesControl FC = FilesControl();
static TxtParser TP = TxtParser();
FilesControl* const SFilesControl = &FC;
TxtParser* const STxtParser = &TP;

File FilesControl::GetFile(std::string_view path)
{
	if (DoesFileExist(path))
	{
		return File(std::string(path));
	}
	return File();
}

File FilesControl::CreateFileByPath(std::string_view path, std::ios_base::openmode mode)
{
	if (!DoesFileExist(path))
	{
		File file{std::string(path), mode};
		file.clear();
		file << "";
		return file;
	}
	else
	{
		SLog->Print("File is already exists!", EMessageType::kWarning);
		return File(std::string(path));
	}
}

inline fs::path FilesControl::GetCurrentPath() const
{
	return fs::current_path();
}

inline bool FilesControl::DoesFileExist(std::string_view path)
{
	return fs::is_block_file(path) || fs::is_character_file(path) || fs::is_regular_file(path);
}

inline bool FilesControl::DoesFolderExist(std::string_view path)
{
	return fs::is_directory(path);
}

bool FilesControl::CreateFolder(std::string_view path)
{
	if (!DoesFolderExist(path))
	{
		return fs::create_directory(path);
	}
	return true;
}

bool FilesControl::GetFileInfo(std::string_view path, std::string& out)
{
	File file =GetFile(path);
	if (file.is_open())
	{
		std::string buf1, result;
		while (file.good())
		{
			file >> buf1;
			result += buf1;
		}
		out = result;
		return true;
	}
	return false;
}

bool TxtParser::GetValue(std::string_view filePath, std::string_view key, std::string& result)
{
	File file = SFilesControl->GetFile(filePath);
	if (file.is_open())
	{
		std::string buf;
		while (getline(file, buf))
		{
			if (buf.starts_with(key))
			{
				result = buf.substr(key.size(), buf.size() - key.size());
				return true;
			}
		}
		SLog->Print("Key to find value has not been found!", EMessageType::kWarning);
	}
	else
	{
		SLog->Print("File has not been found!", EMessageType::kWarning);
	}
	return false;
}

template<class T>
SettingsManager<T>::SettingsManager()
{
	SettingsPath = SFilesControl->GetCurrentPath().string() + "\\config";
}

template<class T>
std::int16_t SettingsManager<T>::CreateSettingsFiles()
{
	SLog->Print("Configs successfuly generated! Quiting....", EMessageType::kOk);
	return kConfigFilesCreated;
}

std::int16_t ServerSettingsManager::CreateSettingsFiles()
{
	if (!SFilesControl->DoesFolderExist(SettingsPath))
	{
		SFilesControl->CreateFolder(SettingsPath);
	}
	std::string serverConfig = SettingsPath + "\\server.txt";
	if (!SFilesControl->DoesFileExist(serverConfig))
	{
		File file = SFilesControl->CreateFileByPath(serverConfig);
		if (file.is_open())
		{
			file << "port: \n";
			file.close();
			return SettingsManager::CreateSettingsFiles();
		}
		else
		{
			SLog->Print("Could not create open config file!");
			return kCouldNotCreateConfigFiles;
		}
	}
	return kConfigFilesExist;
}

void ServerSettingsManager::ParseSettings(std::map<std::string_view, std::string>& values)
{
	File connectionFile = GetServerConfig();
	if (connectionFile.is_open())
	{
		std::string buffer;
		while (getline(connectionFile, buffer))
		{
			for (auto& [key, value] : values)
			{
				if (buffer.starts_with(key))
				{
					value = buffer.substr(key.size() + 2, buffer.size() - key.size());
					remove_spaces(std::move(value));
				}
			}
		}
	}
}

File ServerSettingsManager::GetServerConfig()
{
	std::string connectionConfig = SettingsPath + "\\server.txt";
	if (SFilesControl->DoesFileExist(connectionConfig))
	{
		return SFilesControl->GetFile(connectionConfig);
	}
	return File();
}

std::int16_t ClientSettingsManager::CreateSettingsFiles()
{
	if (!SFilesControl->DoesFolderExist(SettingsPath))
	{
		SFilesControl->CreateFolder(SettingsPath);
	}
	std::string connectionConfig = SettingsPath + "\\connection.txt";
	if (!SFilesControl->DoesFileExist(connectionConfig))
	{
		File file = SFilesControl->CreateFileByPath(connectionConfig);
		if (file.is_open())
		{
			file << "address: \nport: \n";
			file.close();
			return SettingsManager::CreateSettingsFiles();
		}
		else
		{
			SLog->Print("Could not create open config file!");
			return kCouldNotCreateConfigFiles;
		}
	}
	return kConfigFilesExist;
}

void ClientSettingsManager::ParseSettings(std::map<std::string_view, std::string>& values)
{
	File connectionFile = GetConnectionConfig();
	if (connectionFile.is_open())
	{
		std::string buffer;
		while (getline(connectionFile, buffer))
		{
			for (auto& [key, value] : values)
			{
				if (buffer.starts_with(key))
				{
					value = buffer.substr(key.size()+2, buffer.size() - key.size());
					remove_spaces(std::move(value));
				}
			}
		}
	}
}

File ClientSettingsManager::GetConnectionConfig()
{
	std::string connectionConfig = SettingsPath + "\\connection.txt";
	if (SFilesControl->DoesFileExist(connectionConfig))
	{
		return SFilesControl->GetFile(connectionConfig);
	}
	return File();
}

void remove_spaces(std::string&& str)
{
	auto it = std::find(str.begin(), str.end(), ' ');
	while (it != str.end())
	{
		str.erase(it);
		it = std::find(str.begin(), str.end(), ' ');
	}
}
