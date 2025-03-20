#pragma once

#include "imgui.h"
#include "imgui_impl_dx11.h"
#include "imgui_impl_win32.h"
#include "render/IRenderer.hpp"
#include "ResourceManager.hpp"
#include "UIManager.hpp"
#include "utils/Time.hpp"
#include <array>
#include <cstdint>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <intsafe.h>
#include <iostream>
#include <memory>
#include <queue>
#include <stdexcept>
#include <Windows.h>
#include <wrl/client.h>

#define HR_CHECK(hr, msg)                                   \
    if (FAILED(hr)) {                                       \
       throw std::runtime_error(msg); \
    }

using namespace Microsoft::WRL;
using namespace DirectX;

struct Vertex
{
	XMFLOAT3 pos;
};

struct CBuffer {
	XMMATRIX world;
	XMMATRIX view;
	XMMATRIX projection;
};

class D3D11Renderer : public IRenderer
{
public:
	D3D11Renderer(uint32_t width, uint32_t height, bool vsync, HWND hwnd);
	~D3D11Renderer();

	void Start() override;
	void Frame() override;

	ComPtr<ID3D11Device>& GetDevice();
	ComPtr<ID3D11DeviceContext>& GetDeviceContext();

private:
	ComPtr<ID3D11Device> m_device;
	ComPtr<IDXGISwapChain> m_swapchain;
	ComPtr<ID3D11DepthStencilView> m_depth_view;
	ComPtr<ID3D11DeviceContext> m_device_context;
	ComPtr<IDXGIFactory> m_factory;
	ComPtr<ID3D11RenderTargetView> m_rtv;
	ComPtr<ID3D11DepthStencilView> m_depth_stencil_view;

	void InitDevice();
	void InitSwapchain();
	void InitBackBuffer();
	void InitDepth();
	void InitViewport();

	void SetVsync(bool bVsync) override;
	bool vsync;

	// Resrouces
	ResourceManager<ID3D11Buffer> buffer_manager;
	ResourceManager<ID3D11VertexShader> vertex_shader_manager;
	ResourceManager<ID3D11PixelShader> pixel_shader_manager;
	ResourceManager<ID3D11InputLayout> input_layout_manager;

	template<typename T>
	void CreateBuffer(std::vector<T>& data, const std::string& name, D3D11_USAGE usage, UINT bindflag, UINT cpu_access);
	void CreateConstantBuffer(const std::string& name);
	void UpdateCosntantBuffer(const std::string& name, CBuffer& buffer);
	void LoadShader(LPCWSTR file_path, bool vertex, const std::string& name);

	template<int T>
	void CreateInputLayout(std::array<D3D11_INPUT_ELEMENT_DESC, T> vertex_desc, const std::string& name);

	// Temp
	ComPtr<ID3DBlob> vblob;

	HWND m_hwnd;
	uint32_t m_width, m_height;

	std::unique_ptr<UIManager> editor_ui;




	// TODO: Queue of objects to be rendered.
};

