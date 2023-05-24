export module Server;

import "ServerClientCore.h";

export class Server final : public ServerClientCore
{
public:
	void Initialize() override;
};

void Server::Initialize()
{
	ZeroMemory(&AddrInfo, sizeof(AddrInfo));
	AddrInfo.ai_family = AF_INET;
	AddrInfo.ai_socktype = SOCK_STREAM;
	AddrInfo.ai_protocol = IPPROTO_TCP;
	AddrInfo.ai_flags = AI_PASSIVE;
	ServerClientCore::Initialize();
}
