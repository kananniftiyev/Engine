#pragma once

#include <cstdint>
#include <d3d11.h>
#include <d3dcommon.h>
#include <dxgi.h>
#include <intsafe.h>
#include <stdexcept>
#include <Windows.h>
#include <wrl/client.h>

#define HR_CHECK(hr, msg)                                   \
    if (FAILED(hr)) {                                       \
       throw std::runtime_error(msg); \
    }

using namespace Microsoft::WRL;

class D3D11Renderer
{
public:
	D3D11Renderer(uint32_t width, uint32_t height, bool vsync, HWND hwnd);

	void Frame();

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<IDXGISwapChain> m_swapchain;
	ComPtr<ID3D11DepthStencilView> m_depth_view;
	ComPtr<ID3D11DeviceContext> m_device_context;
	ComPtr<IDXGIFactory> m_factory;
	ComPtr<ID3D11RenderTargetView> m_rtv;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;

	void InitDevice();
	void InitSwapchain(uint32_t width, uint32_t height, HWND hwnd);
	void InitBackBuffer();
	void InitDepth(uint32_t width, uint32_t height);
	void InitViewport(uint32_t width, uint32_t height);


};