#pragma once

#include "platform/WindowsImpl.hpp"
#include "Renderer.hpp"
#include "utils/Time.hpp"
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <UIManager.hpp>
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
	GraphicsAPI api;
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
	std::unique_ptr<Renderer> renderer;
	// std::unique_ptr<EngineInputManager> input_manager;



	EngineConfig config;

	// Window

};