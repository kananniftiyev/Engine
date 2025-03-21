#include "App.hpp"



EngineApp::EngineApp(EngineConfig& config) {
	window = std::make_unique<WindowsImpl>(config.witdh, config.height, config.winEntry.hInstance, config.winEntry.nCmdShow);
	renderer = std::make_unique<Renderer>(config.api, config.witdh, config.height, config.vsync, window->GetHwnd());
	// input_manager = std::make_unique<EngineInputManager>();
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
	renderer->Start();

	while (is_running)
	{
		Time::FrameRate();
		window->Message();
		renderer->Run();



		is_running = window->IsWindowClosed();
	}
}

