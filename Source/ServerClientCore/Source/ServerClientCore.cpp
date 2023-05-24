#include "ServerClientCore.h"
#include "Logger.h"

#include <WS2tcpip.h>
#include <format>

#pragma comment(lib, "ws2_32.lib")

bool ServerClientCore::Startup()
{
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		SLog->Print
		(
			std::format("Could not startup the Windows Sockets!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		return false;
	}
	SLog->Print("Successfuly started up!", EMessageType::kOk);
	return true;
}

bool ServerClientCore::Shutdown()
{
	if (WSACleanup() != 0)
	{
		SLog->Print
		(
			std::format("Could not clean up the Windows Sockets!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		return false;
	}
	SLog->Print("Successfuly shutted down!", EMessageType::kOk);
	return true;
}
