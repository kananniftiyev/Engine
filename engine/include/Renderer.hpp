#pragma once

#include "render/D3D11Renderer.hpp"
#include <memory>


enum class GraphicsAPI
{
	NONE,
	DirectX11,
	// OpenGL,
};

class Renderer
{
public:
	Renderer(GraphicsAPI api, uint32_t width, uint32_t height, bool bVsync, HWND hwnd);
	~Renderer();

	void Start();
	void Run();

private:
	std::unique_ptr<D3D11Renderer> d3d11_renderer;
	//std::unique_ptr<UIManager> editor_ui;
};
