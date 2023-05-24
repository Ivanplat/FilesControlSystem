#include "pch.h"
#include "Logger.h"
#include "ServerClientCore.h"

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