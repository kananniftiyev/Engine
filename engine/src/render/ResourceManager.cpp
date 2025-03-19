#include "render/ResourceManager.hpp"

ResourceManager::~ResourceManager()
{
	for (auto [name, buffer] : m_buffers) {
		buffer->Release();
	}

	for (auto [name, vertex] : m_vertex_shaders) {
		vertex->Release();
	}

	for (auto [name, pixel] : m_pixel_shaders) {
		pixel->Release();
	}

	for (auto [name, input] : m_pixel_shaders) {
		input->Release();
	}
}

ID3D11VertexShader* ResourceManager::GetVertexShaders(const std::string& shader_name)
{
	auto vertex_it = m_vertex_shaders.find(shader_name);

	if (vertex_it != m_vertex_shaders.end())
	{
		return vertex_it->second;
	}

	return nullptr;

}

ID3D11PixelShader* ResourceManager::GetPixelShaders(const std::string& shader_name)
{
	auto vertex_it = m_pixel_shaders.find(shader_name);

	if (vertex_it != m_pixel_shaders.end())
	{
		return vertex_it->second;
	}

	return nullptr;
}

ID3D11Buffer* ResourceManager::GetBuffer(const std::string& buffer_name)
{
	auto buffer_it = m_buffers.find(buffer_name);

	if (buffer_it != m_buffers.end())
	{
		return buffer_it->second;
	}

	return nullptr;
}

ID3D11InputLayout* ResourceManager::GetInputLayout(const std::string& name)
{
	auto layout_it = m_input_layouts.find(name);

	if (layout_it != m_input_layouts.end())
	{
		return layout_it->second;
	}

	return nullptr;
}

void ResourceManager::AddVertexShader(const std::string& name, ID3D11VertexShader* shader)
{
	m_vertex_shaders[name] = shader;
}

void ResourceManager::AddPixelShader(const std::string& name, ID3D11PixelShader* shader)
{
	m_pixel_shaders[name] = shader;
}

void ResourceManager::AddBuffer(const std::string& name, ID3D11Buffer* buffer)
{
	m_buffers[name] = buffer;
}

void ResourceManager::AddInputLayout(ID3D11InputLayout* input, const std::string& name)
{
	m_input_layouts[name] = input;
}
