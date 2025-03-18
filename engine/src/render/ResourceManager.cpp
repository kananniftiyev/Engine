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
}

ID3D11VertexShader* ResourceManager::GetVertexShaders(std::string shader_name)
{
	auto vertex_it = m_vertex_shaders.find(shader_name);

	if (vertex_it != m_vertex_shaders.end())
	{
		return vertex_it->second;
	}

	return nullptr;

}

ID3D11PixelShader* ResourceManager::GetPixelShaders(std::string shader_name)
{
	auto vertex_it = m_pixel_shaders.find(shader_name);

	if (vertex_it != m_pixel_shaders.end())
	{
		return vertex_it->second;
	}

	return nullptr;
}

ID3D11Buffer* ResourceManager::GetBuffer(std::string buffer_name)
{
	auto buffer_it = m_buffers.find(buffer_name);

	if (buffer_it != m_buffers.end())
	{
		return buffer_it->second;
	}

	return nullptr;
}

void ResourceManager::AddVertexShader(std::string name, ID3D11VertexShader* shader)
{
	m_vertex_shaders[name] = shader;
}

void ResourceManager::AddPixelShader(std::string name, ID3D11PixelShader* shader)
{
	m_pixel_shaders[name] = shader;
}

void ResourceManager::AddBuffer(std::string name, ID3D11Buffer* buffer)
{
	m_buffers[name] = buffer;
}
