#include "ServerClientCore.h"
#include "Logger.h"
#include "FilesControl.h"

#include <format>
#include <thread>
#include <mutex>
#include <future>
#include <memory>

#pragma comment(lib, "ws2_32.lib")

static std::mutex mtx, mtx2;

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
	Socket = socket(AddrResult->ai_family, AddrResult->ai_socktype, AddrResult->ai_protocol);
	if (Socket == INVALID_SOCKET)
	{
		SLog->Print
		(
			std::format("Could not create a socket!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
		throw std::exception("WS Initialization error!");
	}
}

void ServerClientCore::RecieveFile(SOCKET rvSocket)
{
	Data file = Data(100000);
	std::int32_t result = recv(rvSocket, (char*)file.data(), file.size(), 0);
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not recieve from SOCKET {}!", rvSocket),
			EMessageType::kWarning
		);
		return;
	}
	fs::path currentPath = SFilesControl->GetCurrentPath();
	std::string filePath = currentPath.string() + "\\testfile.txt";
	std::fstream fileStream = SFilesControl->CreateFileByPath(filePath, std::ios::app);
	fileStream << std::string(file.begin(), file.end());
	fileStream.close();
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

ServerCore::ServerCore()
{
	SettingManagerFactory factory;
	auto managerPtr = factory.ConstructSettingsManager<ServerSettingsManager, ServerSettings>();
	std::int16_t result = managerPtr->CreateSettingsFiles();
	if (result == kConfigFilesExist)
	{
		std::map<std::string_view, std::string> settings = { {"address", ""}, {"port", ""} };
		managerPtr->ParseSettings(settings);
		Port = settings["port"];
		if (Port == "")
		{
			SLog->Print("Server port is not set properly!", EMessageType::kError);
			throw std::exception("Could not parse config files!");
			exit(-1);
		}
	}
	else if (result == kConfigFilesCreated)
	{
		exit(0);
	}
	else if (result == kCouldNotCreateConfigFiles)
	{
		throw std::exception("Could not create config files!");
		exit(-1);
	}
	managerPtr.reset();
}

void ServerCore::Initialize()
{
	ZeroMemory(&AddrInfo, sizeof(AddrInfo));
	AddrInfo.ai_family = AF_INET;
	AddrInfo.ai_socktype = SOCK_STREAM;
	AddrInfo.ai_protocol = IPPROTO_TCP;
	AddrInfo.ai_flags = AI_PASSIVE;
	std::int32_t result = getaddrinfo(nullptr, Port.c_str(), &AddrInfo, &AddrResult);
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
	result = bind(Socket, AddrResult->ai_addr, static_cast<int>(AddrResult->ai_addrlen));
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not bind a socket!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
		throw std::exception("Could not bind a socket!");
	}
	result = listen(Socket, SOMAXCONN);
	if (result == SOCKET_ERROR)
	{
		SLog->Print
		(
			std::format("Could not listen a socket!\n Last WS error: {}", WSAGetLastError()),
			EMessageType::kError
		);
		Shutdown();
		throw std::exception("Could not listen a socket!");
	}
}

void ServerCore::CheckNewConnections(SOCKET ServerSocket)
{
	const std::lock_guard<std::mutex> guard(mtx);
	while (true)
	{
		SLog->Print("CHECK", EMessageType::kOk);
		SOCKET userSocket = accept(ServerSocket, nullptr, nullptr);
		if (userSocket != NULL)
		{
			std::future handle = std::async(std::launch::async, [&]() {return this->HandleUser(userSocket); });
			handle.get();
		}
		std::this_thread::sleep_for(std::chrono::seconds(1));
	}
}

void ServerCore::HandleUser(SOCKET user)
{
	while (true)
	{
		Data command = Data(128);
		std::int32_t result = recv(user, (char*)command.data(), command.size(), 0);
		if (result == SOCKET_ERROR)
		{
			SLog->Print
			(
				std::format("User SOCKET {} disconnected!", user),
				EMessageType::kOk
			);
			return;
		}
		std::string commandStr(command.begin(), command.end());
		if (commandStr.starts_with("send file"))
		{
			send(user, "Ok", 2, 0);
			RecieveFile(user);
		}
		SLog->Print(std::string(command.begin(), command.end()));
	}
}

void ServerCore::MainLoop()
{
	std::thread checkTread(&ServerCore::CheckNewConnections, this, Socket);
	std::thread mainThread([&]()
		{
			const std::lock_guard<std::mutex> guard(mtx2);
			while (true)
			{
				std::this_thread::sleep_for(std::chrono::seconds(1));
			}
		});
	checkTread.detach();
	mainThread.join();
}

ClientCore::ClientCore()
{
	SettingManagerFactory factory;
	auto managerPtr = factory.ConstructSettingsManager<ClientSettingsManager, ClientSettings>();
	if (managerPtr != nullptr)
	{
		std::int16_t result = managerPtr->CreateSettingsFiles();
		if (result == kConfigFilesExist)
		{
			std::map<std::string_view, std::string> settings = { {"address", ""}, {"port", ""} };
			managerPtr->ParseSettings(settings);
			Address = settings["address"];
			Port = settings["port"];
			if (Port == "" || Address == "")
			{
				SLog->Print("Server address and/or server port are not set properly!", EMessageType::kError);
				throw std::exception("Could not parse config files!");
				exit(-1);
			}
		}
		else if (result == kConfigFilesCreated)
		{
			exit(0);
		}
		else if(result == kCouldNotCreateConfigFiles)
		{
			throw std::exception("Could not create config files!");
			exit(-1);
		}
	}
	managerPtr.reset();
}
