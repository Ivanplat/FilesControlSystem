#include "Client.h"
#include "Logger.h"
#include "FilesControl.h"

#include <format>
#include <memory>

void Client::Initialize()
{
	ZeroMemory(&AddrInfo, sizeof(AddrInfo));
	AddrInfo.ai_family = AF_INET;
	AddrInfo.ai_socktype = SOCK_STREAM;
	AddrInfo.ai_protocol = IPPROTO_TCP;
	std::int32_t result = getaddrinfo(Address.c_str(), Port.c_str(), &AddrInfo, &AddrResult);
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not initalize WSA!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
		throw std::exception("WSA Initialization error!");
	}
	ServerClientCore::Initialize();
}

bool Client::ConnectToServer()
{
	std::int32_t result = connect(Socket, AddrResult->ai_addr, static_cast<int>(AddrResult->ai_addrlen));
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not connect to the server!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		return false;
	}
	return true;
}

void Client::SendFileToServer()
{
	std::int32_t result = send(Socket, "send file", 10, 0);
	if (result == SOCKET_ERROR)
	{
		
	}
	std::string fileInfo;
	Data data = Data(128);
	if (recv(Socket, (char*)data.data(), data.size(), 0) != -1)
	{
		SLog->Print(std::string(data.begin(), data.end()), EMessageType::kOk);
		if (SFilesControl->GetFileInfo("C:\\Users\\EventGraph\\source\\repos\\FilesControlSystem\\Source\\FilesControl\\MoveDll.ps1", fileInfo))
		{
			result = send(Socket, fileInfo.c_str(), fileInfo.size(), 0);
		}
	}
}
