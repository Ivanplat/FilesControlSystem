#pragma once

#ifdef __EXPORT_UICORE_DLL
#define UICORE_API __declspec(dllexport)
#else
#define UICORE_API __declspec(dllimport)
#endif

class Form {};
struct Size {};

class UICORE_API UICore
{
public:
	Form CreateWindowForm(Size formSize);
};