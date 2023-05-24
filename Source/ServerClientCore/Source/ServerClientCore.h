#pragma once

class ServerClientCore
{
public:
	explicit ServerClientCore() noexcept = default;
	virtual ~ServerClientCore() noexcept = default;

public:
	virtual bool Startup();
	virtual bool Shutdown();
};