#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "UIManager.hpp"
#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>

using namespace Microsoft::WRL;

class UIManager
{
public:
	UIManager(HWND hwnd, ComPtr<ID3D11Device> device, ComPtr<ID3D11DeviceContext> device_context);
	~UIManager();

	void Frame();

private:

	void TopBar();
	void DiagnosticWindow();

};