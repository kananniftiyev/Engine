#pragma once

#include "ResourceManager.hpp"
#include "utils/Time.hpp"
#include <cstdint>
#include <d3d11.h>
#include <d3dcommon.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <dxgi.h>
#include <intsafe.h>
#include <iostream>
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

class D3D11Renderer
{
public:
	D3D11Renderer(uint32_t width, uint32_t height, bool vsync, HWND hwnd);
	~D3D11Renderer();

	void Start();
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
	void InitSwapchain();
	void InitBackBuffer();
	void InitDepth();
	void InitViewport();

	// Resrouces
	ResourceManager resource_manager;
	void CreateBuffer(std::vector<Vertex>& data, std::string name);
	void CreateIndexBuffer(std::vector<WORD>& data, std::string name);
	void CreateConstantBuffer(std::string name);
	void UpdateCosntantBuffer(std::string name, CBuffer& buffer);
	void LoadShader(LPCWSTR file_path, bool vertex, std::string name);

	// Temp
	ID3DBlob* vblob;

	HWND m_hwnd;
	uint32_t m_width, m_height;

	// TODO: Queue of objects to be rendered.
};