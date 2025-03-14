#include "App.hpp"
#include <exception>
#include <memory>
#include <platform/WindowsImpl.hpp>
#include <Windows.h>


EngineApp::EngineApp(EngineConfig& config) {
	window = std::make_unique<WindowsImpl>(config.witdh, config.height, config.winEntry.hInstance, config.winEntry.nCmdShow);
}

EngineApp::~EngineApp() {

}

int EngineApp::Run(EngineConfig& config) {
	EngineApp engine(config);

	try
	{
		engine.MainLoop();
	}
	catch (const std::exception& e)
	{
		MessageBoxA(nullptr, static_cast<LPCSTR>(e.what()), static_cast<LPCSTR>(e.what()), MB_OK | MB_ICONERROR);
		return -1;
	}

	return 0;
}


void EngineApp::MainLoop() {
	while (is_running)
	{
		window->Message();

		is_running = window->IsWindowClosed();
	}
}

