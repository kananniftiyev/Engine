#include "render/D3D11Renderer.hpp"

D3D11Renderer::D3D11Renderer(uint32_t width, uint32_t height, bool vsync, HWND hwnd)
{
	m_width = width;
	m_height = height;
	m_hwnd = hwnd;
	InitDevice();
	InitSwapchain();
	InitBackBuffer();
	InitDepth();
	InitViewport();
	editor_ui = std::make_unique<UIManager>(hwnd, m_device, m_device_context);

	// TODO: Depth Stencil state
}

D3D11Renderer::~D3D11Renderer()
{
}

void D3D11Renderer::Start()
{
	std::vector<Vertex> vertices =
	{
		{ XMFLOAT3(-1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, 1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, -1.0f) },
		{ XMFLOAT3(1.0f, -1.0f, 1.0f) },
		{ XMFLOAT3(-1.0f, -1.0f, 1.0f) },
	};

	std::vector<WORD> indices =
	{
		3, 1, 0,
		2, 1, 3,

		0, 5, 4,
		1, 5, 0,

		3, 4, 7,
		0, 4, 3,

		1, 6, 5,
		2, 6, 1,

		2, 7, 6,
		3, 7, 2,

		6, 4, 5,
		7, 4, 6,
	};

	LoadShader(L"assets/shaders/cube/cube_vertex.hlsl", true, "vert");
	LoadShader(L"assets/shaders/cube/cube_pixel.hlsl", false, "pix");

	CreateBuffer<Vertex>(vertices, "vert_b", D3D11_USAGE_IMMUTABLE, D3D11_BIND_VERTEX_BUFFER, 0);
	CreateBuffer<WORD>(indices, "indices_b", D3D11_USAGE_IMMUTABLE, D3D11_BIND_INDEX_BUFFER, 0);
	CreateConstantBuffer("c_b");

	std::array<D3D11_INPUT_ELEMENT_DESC, 1> vertex_desc = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	CreateInputLayout<1>(vertex_desc, "object");

	editor_ui->resource_stats.buffer_count = buffer_manager.CacheCount();
	editor_ui->resource_stats.v_shader_count = vertex_shader_manager.CacheCount();
	editor_ui->resource_stats.p_shader_count = pixel_shader_manager.CacheCount();
	editor_ui->resource_stats.input_count = input_layout_manager.CacheCount();
}

void D3D11Renderer::Frame()
{
	/*auto res = Time::FrameTime();
	std::cout << "Graphics Render Time: " << res << "\n";*/
	static FLOAT clearColor[4] = { 0.3f, 0.5f, 0.7f, 1.0f };

	m_device_context->ClearRenderTargetView(m_rtv.Get(), clearColor);
	m_device_context->ClearDepthStencilView(m_depth_stencil_view.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	m_device_context->VSSetShader(vertex_shader_manager.Get("vert").Get(), nullptr, 0);
	m_device_context->PSSetShader(pixel_shader_manager.Get("pix").Get(), nullptr, 0);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	ID3D11Buffer* vertexBuffer = buffer_manager.Get("vert_b").Get();
	auto c_buffer = buffer_manager.Get("c_b").Get();

	CBuffer cb;

	static float angle = 0.0f;
	auto model = XMMatrixRotationY(angle);
	angle += 0.0001f;

	XMVECTOR Eye = XMVectorSet(0.0f, 0.0f, -3.0f, 0.0f);
	XMVECTOR At = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	XMVECTOR Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	auto view = XMMatrixLookAtLH(Eye, At, Up);

	static float ratio = static_cast<float>(m_width) / static_cast<float>(m_height);
	auto projection = XMMatrixPerspectiveFovLH(XM_PIDIV2, ratio, 1.0f, 100.0f);

	cb.projection = projection;
	cb.view = view;
	cb.world = model;
	UpdateCosntantBuffer("c_b", cb);


	m_device_context->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	m_device_context->IASetIndexBuffer(buffer_manager.Get("indices_b").Get(), DXGI_FORMAT_R16_UINT, 0);
	m_device_context->VSSetConstantBuffers(0, 1, &c_buffer);


	m_device_context->IASetInputLayout(input_layout_manager.Get("object").Get());

	m_device_context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	m_device_context->DrawIndexed(36, 0, 0);

	editor_ui->frame_stats.fps = Time::FrameRate();
	editor_ui->frame_stats.render_ms = Time::FrameTime();

	editor_ui->Frame();

	m_swapchain->Present(0, 0);
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

void D3D11Renderer::InitSwapchain()
{
	DXGI_SWAP_CHAIN_DESC desc{};
	desc.BufferDesc.Width = m_width;
	desc.BufferDesc.Height = m_height;
	desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	desc.BufferCount = 2;
	desc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	desc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	desc.BufferDesc.RefreshRate.Denominator = 0;
	desc.BufferDesc.RefreshRate.Numerator = 0;
	desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.OutputWindow = m_hwnd;
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

void D3D11Renderer::InitDepth()
{
	D3D11_TEXTURE2D_DESC desc{};
	desc.Width = m_width;
	desc.Height = m_height;
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

	//D3D11_DEPTH_STENCIL_DESC dsd{};
	//dsd.DepthEnable = true;
	//dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	//dsd.StencilEnable = true;
	//dsd.

	hr = m_device->CreateDepthStencilView(texture.Get(), nullptr, m_depth_stencil_view.GetAddressOf());

	HR_CHECK(hr, "Could not create depth view.");

	m_device_context->OMSetRenderTargets(1, m_rtv.GetAddressOf(), m_depth_stencil_view.Get());

}

void D3D11Renderer::InitViewport()
{
	D3D11_VIEWPORT vp{};
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;
	vp.Width = static_cast<float>(m_width);
	vp.Height = static_cast<float>(m_height);
	vp.MaxDepth = 1.0f;
	vp.MinDepth = 0.0f;

	m_device_context->RSSetViewports(1, &vp);
}

template<typename T>
void D3D11Renderer::CreateBuffer(std::vector<T>& data, const std::string& name, D3D11_USAGE usage, UINT bindflag, UINT cpu_access)
{
	D3D11_BUFFER_DESC bdesc{};
	bdesc.ByteWidth = sizeof(T) * data.size();
	bdesc.Usage = usage;
	bdesc.BindFlags = bindflag;
	bdesc.CPUAccessFlags = cpu_access;
	bdesc.MiscFlags = 0;
	bdesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA sdesc{};
	sdesc.pSysMem = data.data();

	ComPtr<ID3D11Buffer> buffer = nullptr;

	auto hr = m_device->CreateBuffer(&bdesc, &sdesc, buffer.GetAddressOf());

	HR_CHECK(hr, "Could not create buffer");

	buffer_manager.Load(name, buffer);
}

void D3D11Renderer::CreateConstantBuffer(const std::string& name)
{
	D3D11_BUFFER_DESC bdesc{};
	bdesc.ByteWidth = sizeof(CBuffer);
	bdesc.Usage = D3D11_USAGE_DYNAMIC;
	bdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bdesc.MiscFlags = 0;
	bdesc.StructureByteStride = 0;

	ComPtr<ID3D11Buffer> buffer = nullptr;

	auto hr = m_device->CreateBuffer(&bdesc, nullptr, buffer.GetAddressOf());

	HR_CHECK(hr, "Could not create buffer");

	buffer_manager.Load(name, buffer);
}

void D3D11Renderer::UpdateCosntantBuffer(const std::string& name, CBuffer& buffer)
{
	auto c_buffer = buffer_manager.Get(name).Get();

	D3D11_MAPPED_SUBRESOURCE mapped_resource;
	auto hr = m_device_context->Map(c_buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mapped_resource);
	if (SUCCEEDED(hr))
	{

		memcpy(mapped_resource.pData, &buffer, sizeof(CBuffer));
		m_device_context->Unmap(c_buffer, 0);
	}

}

void D3D11Renderer::LoadShader(LPCWSTR file_path, bool vertex, const std::string& name)
{
	ComPtr<ID3DBlob> blob;
	ID3DBlob* eblob;
	auto hr = D3DCompileFromFile(file_path, nullptr, nullptr, "main", (vertex) ? "vs_5_0" : "ps_5_0", D3DCOMPILE_DEBUG, 0, blob.GetAddressOf(), &eblob);


	if (FAILED(hr)) {
		if (eblob) {
			OutputDebugStringA((char*)eblob->GetBufferPointer());

			printf("Shader Compilation Failed: %s\n", (char*)eblob->GetBufferPointer());

			eblob->Release();
		}

	}

	HR_CHECK(hr, "asd");


	if (vertex)
	{
		ComPtr<ID3D11VertexShader> vertex_shader;
		hr = m_device->CreateVertexShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, vertex_shader.GetAddressOf());

		HR_CHECK(hr, "Could not create vertex shader");

		vertex_shader_manager.Load(name, vertex_shader);

		vblob = blob;
	}
	else {
		ComPtr<ID3D11PixelShader> pixel_shader;
		hr = m_device->CreatePixelShader(blob->GetBufferPointer(), blob->GetBufferSize(), nullptr, pixel_shader.GetAddressOf());

		HR_CHECK(hr, "Could not create pixel shader");

		pixel_shader_manager.Load(name, pixel_shader);
	}
}

ComPtr<ID3D11Device>& D3D11Renderer::GetDevice()
{
	return m_device;
}

ComPtr<ID3D11DeviceContext>& D3D11Renderer::GetDeviceContext()
{
	return m_device_context;
}

template<int T>
void D3D11Renderer::CreateInputLayout(std::array<D3D11_INPUT_ELEMENT_DESC, T> vertex_desc, const std::string& name)
{
	ComPtr<ID3D11InputLayout> input_layout;
	auto hr = m_device->CreateInputLayout(vertex_desc.data(), T, vblob->GetBufferPointer(), vblob->GetBufferSize(), input_layout.GetAddressOf());

	HR_CHECK(hr, "Could not create input layout");
	input_layout_manager.Load(name, input_layout);
}