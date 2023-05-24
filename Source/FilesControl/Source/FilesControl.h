#pragma once

#ifdef __EXPORT_DLL
#define FILESCONTROL_API __declspec(dllexport)
#else
#define FILESCONTROL_API __declspec(dllimport)
#endif

class FILESCONTROL_API FilesControl
{
public:
	
};

extern FILESCONTROL_API FilesControl* const SFilesControl;