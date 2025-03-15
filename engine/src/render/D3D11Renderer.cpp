#include "render/D3D11Renderer.hpp"

D3D11Renderer::D3D11Renderer(uint32_t width, uint32_t height, bool vsync, HWND hwnd)
{
	InitDevice();
	InitSwapchain(width, height, hwnd);
	InitBackBuffer();
	InitDepth(width, height);
	InitViewport(width, height);
}

void D3D11Renderer::Frame()
{
	static FLOAT clearColor[4] = { 0.3f, 0.5f, 0.7f, 1.0f };

	m_device_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_swapchain->Present(1, 0);
}

void D3D11Renderer::InitDevice()
{
	D3D_FEATURE_LEVEL features[]{
		 D3D_FEATURE_LEVEL_11_0,
	D3D_FEATURE_LEVEL_10_1,
	D3D_FEATURE_LEVEL_10_0,
	D3D_FEATURE_LEVEL_9_3,
	D3D_FEATURE_LEVEL_9_2,
	D3D_FEATURE_LEVEL_9_1,
	};

	auto hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, D3D11_CREATE_DEVICE_DEBUG,
		features, ARRAYSIZE(features), D3D11_SDK_VERSION, m_device.GetAddressOf(), nullptr, m_device_context.GetAddressOf());

	HR_CHECK(hr, "Could not create device");

}

void D3D11Renderer::InitSwapchain(uint32_t width, uint32_t height, HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferDesc.Width = width;
	desc.BufferDesc.Height = height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferCount = 2;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = hwnd;
	desc.Windowed = TRUE;
	desc.SwapEffect = DXGI_SWAP_EFFECT_SEQUENTIAL;
	desc.Flags = 0;


	auto hr = CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(m_factory.GetAddressOf()));

	HR_CHECK(hr, "Could not create DXGI factory");

	hr = m_factory->CreateSwapChain(m_device.Get(), &desc, m_swapchain.GetAddressOf());

	HR_CHECK(hr, "Could not create Swapchain");

}

void D3D11Renderer::InitBackBuffer()
{
	ID3D11Texture2D* back_buffer = nullptr;
	auto hr = m_swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&back_buffer));

	HR_CHECK(hr, "Could not get buffer.");

	hr = m_device->CreateRenderTargetView(back_buffer, 0, m_rtv.GetAddressOf());

	HR_CHECK(hr, "Could not create render target view."); //error throws here 

	back_buffer->Release();

}

void D3D11Renderer::InitDepth(uint32_t width, uint32_t height)
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = width;
	desc.Height = height;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	ComPtr<ID3D11Texture2D> texture;

	auto hr = m_device->CreateTexture2D(&desc, nullptr, texture.GetAddressOf());

	HR_CHECK(hr, "Could not create depth texture.");

	hr = m_device->CreateDepthStencilView(texture.Get(), nullptr, m_depth_stencil_view.GetAddressOf());

	HR_CHECK(hr, "Could not create depth view.");

	m_device_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_depth_stencil_view.Get());

}

void D3D11Renderer::InitViewport(uint32_t width, uint32_t height)
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(width);
	vp.Height = static_cast<float>(height);
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;

	m_device_context->RSSetViewports(1, &vp);
}
