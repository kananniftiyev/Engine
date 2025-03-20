#include "Renderer.hpp"

Renderer::Renderer(GraphicsAPI api, uint32_t width, uint32_t height, bool bVsync, HWND hwnd)
{
	if (api == GraphicsAPI::DirectX11)
	{
		d3d11_renderer = std::make_unique<D3D11Renderer>(width, height, bVsync, hwnd);
		//editor_ui = std::make_unique<UIManager>(hwnd, d3d11_renderer->GetDevice(), d3d11_renderer->GetDeviceContext());
	}
	else if (api == GraphicsAPI::NONE) {
		abort();
	}
}

Renderer::~Renderer()
{
}

void Renderer::Start() {
	d3d11_renderer->Start();
}

void Renderer::Run() {
	d3d11_renderer->Frame();
}
