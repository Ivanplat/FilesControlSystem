export module Client;

import <string>;
import "ServerClientCore.h";

export class Client final : public ServerClientCore
{
public:
	void Initialize() override;
private:

};

void Client::Initialize()
{
	ZeroMemory(&AddrInfo, sizeof(AddrInfo));
	AddrInfo.ai_family = AF_INET;
	AddrInfo.ai_socktype = SOCK_STREAM;
	AddrInfo.ai_protocol = IPPROTO_TCP;
	ServerClientCore::Initialize();
}
