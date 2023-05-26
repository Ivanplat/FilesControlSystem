#include <string>
#include "ServerClientCore.h"
#include <WS2tcpip.h>

class Client final : public ClientCore
{
public:
	void Initialize() override;
	bool ConnectToServer();
	void SendFileToServer();
private:
};
