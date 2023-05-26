#pragma once

#include <string>
#include <vector>
#include <WS2tcpip.h>

typedef std::vector<std::uint8_t> Data;

struct ConnectedUserInfo
{
	SOCKET UserSocket;
};

class ServerClientCore
{
public:
	explicit ServerClientCore() noexcept = default;
	virtual ~ServerClientCore() noexcept = default;

public:
	virtual bool Startup();
	virtual bool Shutdown();
	virtual void Initialize();
	virtual void RecieveFile(SOCKET rvSocket);

protected:
	virtual void GetAdressAndPortInfo();
	virtual void Send(const Data& info);
	virtual Data Recieve();

protected:
	std::string Address;
	std::string Port;
	SOCKET Socket;
	ADDRINFO AddrInfo;
	ADDRINFO* AddrResult = nullptr;
};

class ServerCore : public ServerClientCore
{
public:
	explicit ServerCore();

public:
	virtual void Initialize() override;
	virtual void CheckNewConnections(SOCKET ServerSocket);
	virtual void HandleUser(SOCKET user);
	virtual void MainLoop();
};

class ClientCore : public ServerClientCore
{
public:
	explicit ClientCore();
};