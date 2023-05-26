#include "FilesControl.h"
#include "Logger.h"

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

File FilesControl::CreateFile(std::string_view path, std::ios_base::openmode mode)
{
	if (!DoesFileExist(path))
	{
		File file{std::string(path), mode};
		file.clear();
		file << "";
		file.close();
	}
	else
	{
		SLog->Print("File is already exists!", EMessageType::kWarning);
		return File(std::string(path));
	}
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
	if (DoesFolderExist(path))
	{
		fs::create_directory(path);
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
