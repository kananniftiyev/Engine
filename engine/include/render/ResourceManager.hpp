#pragma once

#include <d3d11.h>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>


struct ShaderData
{
	ID3D11VertexShader* vertex_shader;
	ID3D11PixelShader* pixel_shader;
};

class ResourceManager
{
public:
	~ResourceManager();

	ID3D11VertexShader* GetVertexShaders(std::string shader_name);
	ID3D11PixelShader* GetPixelShaders(std::string shader_name);
	ID3D11Buffer* GetBuffer(std::string buffer_name);

	void AddVertexShader(std::string name, ID3D11VertexShader* shader);
	void AddPixelShader(std::string name, ID3D11PixelShader* shader);

	void AddBuffer(std::string name, ID3D11Buffer* buffer);


private:
	std::unordered_map<std::string, ID3D11VertexShader*> m_vertex_shaders;
	std::unordered_map<std::string, ID3D11PixelShader*> m_pixel_shaders;
	std::unordered_map<std::string, ID3D11Buffer*> m_buffers;

	// TODO: Lookup shader file path ref.
};
