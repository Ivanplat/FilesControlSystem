#pragma once

#include <string>
#include <vector>
#include <WS2tcpip.h>

typedef std::vector<std::uint8_t> Data;

class ServerClientCore
{
public:
	explicit ServerClientCore() noexcept = default;
	virtual ~ServerClientCore() noexcept = default;

public:
	virtual bool Startup();
	virtual bool Shutdown();
	virtual void Initialize();

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