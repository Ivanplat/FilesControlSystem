#include "Server.h"

int main()
{
	Server server;
	server.Startup();
	server.Initialize();
	server.MainLoop();
	server.Shutdown();
	return 0;
}