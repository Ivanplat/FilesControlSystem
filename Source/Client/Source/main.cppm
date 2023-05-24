import "Logger.h";
import Client;


int main()
{
	Client cl = Client();
	cl.Startup();
	cl.Initialize();
	cl.Shutdown();
}