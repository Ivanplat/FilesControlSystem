#include "ServerClientCore.h"
#include "Logger.h"
#include "FilesControl.h"

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
	closesocket(Socket);
	freeaddrinfo(AddrResult);
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

void ServerClientCore::Initialize()
{
	std::int32_t result = getaddrinfo(Address.c_str(), Port.c_str(), &AddrInfo, &AddrResult);
	if (result != 0)
	{
		SLog->Print
		(
			std::format("Could not initalize WSA!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
		throw std::exception("WSA Initialization error!");
	}
}

void ServerClientCore::GetAdressAndPortInfo()
{
	SFilesControl;
}

void ServerClientCore::Send(const Data& info)
{
	std::int32_t result = send(Socket, (char*)info.data(), info.size(), 0);
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not send data! \n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
	}
}

Data ServerClientCore::Recieve()
{
	Data buffer = Data(1024);
	std::int32_t result = recv(Socket, (char*)buffer.data(), buffer.size(), 0);
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not recieve data! \n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
	}
	return buffer;
}
