#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "implot.h"
#include "UIManager.hpp"
#include <d3d11.h>
#include <vector>
#include <Windows.h>
#include <wrl.h>

using namespace Microsoft::WRL;

struct ResourceStats {
	int buffer_count = 0;
	int v_shader_count = 0;
	int p_shader_count = 0;
	int input_count = 0;
};

struct FrameStats {
	int fps;
	float render_ms;
	std::vector<int> data;
};

struct SceneStats {

};

class UIManager
{
public:
	UIManager(HWND hwnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> device_context);
	~UIManager();

	void Frame();

	ResourceStats resource_stats;
	FrameStats frame_stats;

private:

	void TopBar();
	void DiagnosticWindow();
	void InspectionWindow();
	void ObjectListWindow();

	void SetupImGuiStyle(bool bStyleDark, float alpha);


};