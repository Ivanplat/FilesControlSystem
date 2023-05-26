#include "Logger.h"
#include "Client.h"


int main()
{
	Client cl = Client();
	cl.Startup();
	cl.Initialize();
	cl.ConnectToServer();
	cl.SendFileToServer();
	cl.Shutdown();
	system("pause");
}