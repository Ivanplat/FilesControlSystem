#include "ServerClientCore.h"
#include "Logger.h"

#include <WS2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

bool ServerClientCore::Initialize()
{
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
	{
		SLog->Print("Could not startup the Windows Sockets!", EMessageType::kError);
		return false;
	}
	return true;
}
