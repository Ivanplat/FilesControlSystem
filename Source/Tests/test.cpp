#include "pch.h"
#include "Logger.h"
#include "ServerClientCore.h"
#include "Server.h"
#include "Client.h"

TEST(LoggerTests, ExistanceTesting)
{
	ASSERT_NE(SLog, nullptr);
}

TEST(ServerClientCoreTest, StartingupTest)
{
	ServerClientCore scc;
	ASSERT_EQ(scc.Startup(), true);
}

TEST(ServerClientCoreTest, ShuttingdownTest)
{
	ServerClientCore scc;
	if (scc.Startup())
	{
		ASSERT_EQ(scc.Shutdown(), true);
	}
	else
	{
		ASSERT_FALSE(true);
	}
}
TEST(ServerClientCoreTest, InitializeTest)
{
	ServerClientCore scc;
	if (scc.Startup())
	{
		ASSERT_ANY_THROW(scc.Initialize());
		scc.Shutdown();
	}
	else
	{
		ASSERT_FALSE(true);
	}
}

TEST(ServerTest, StartingupTest)
{
	Server server;
	ASSERT_EQ(server.Startup(), true);
}

TEST(ServerTest, ShuttingdownTest)
{
	Server server;
	if (server.Startup())
	{
		ASSERT_EQ(server.Shutdown(), true);
	}
	else
	{
		ASSERT_FALSE(true);
	}
}
TEST(ServerTest, InitializeTest)
{
	Server server;
	if (server.Startup())
	{
		ASSERT_NO_THROW(server.Initialize());
		server.Shutdown();
	}
	else
	{
		ASSERT_FALSE(true);
	}
}

TEST(ClientTest, StartingupTest)
{
	Client client;
	ASSERT_EQ(client.Startup(), true);
}

TEST(ClientTest, ShuttingdownTest)
{
	Client client;
	if (client.Startup())
	{
		ASSERT_EQ(client.Shutdown(), true);
	}
	else
	{
		ASSERT_FALSE(true);
	}
}
TEST(ClientTest, InitializeTest)
{
	Client client;
	if (client.Startup())
	{
		ASSERT_NO_THROW(client.Initialize());
		client.Shutdown();
	}
	else
	{
		ASSERT_FALSE(true);
	}
}