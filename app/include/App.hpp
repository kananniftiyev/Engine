#pragma once

#include "platform/WindowsImpl.hpp"
#include <cstdint>
#include <memory>
#include <Windows.h>

struct WinEntry {
	HINSTANCE hInstance;
	int nCmdShow;
};

struct EngineConfig
{
	uint32_t witdh = 800;
	uint32_t height = 600;
	bool fullscreen = false;
	bool vsync = true;
	WinEntry winEntry;
};

class EngineApp
{
public:
	EngineApp(EngineConfig& config);
	~EngineApp();

	static int Run(EngineConfig& config);
	void MainLoop();


private:
	bool is_running;

	std::unique_ptr<WindowsImpl> window;




	EngineConfig config;

	// Window

};