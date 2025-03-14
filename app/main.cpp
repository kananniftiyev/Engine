#include "App.hpp"
#include <windows.h>


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {

	EngineConfig config;
	config.winEntry.hInstance = hInstance;
	config.winEntry.nCmdShow = nCmdShow;

	return EngineApp::Run(config);
}
