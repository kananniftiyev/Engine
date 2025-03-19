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

	ID3D11VertexShader* GetVertexShaders(const std::string& shader_name);
	ID3D11PixelShader* GetPixelShaders(const std::string& shader_name);
	ID3D11Buffer* GetBuffer(const std::string& buffer_name);
	ID3D11InputLayout* GetInputLayout(const std::string& name);

	void AddVertexShader(const std::string& name, ID3D11VertexShader* shader);
	void AddPixelShader(const std::string& name, ID3D11PixelShader* shader);

	void AddBuffer(const std::string& name, ID3D11Buffer* buffer);
	void AddInputLayout(ID3D11InputLayout* input, const std::string& name);


private:
	std::unordered_map<std::string, ID3D11VertexShader*> m_vertex_shaders;
	std::unordered_map<std::string, ID3D11PixelShader*> m_pixel_shaders;
	std::unordered_map<std::string, ID3D11Buffer*> m_buffers;
	std::unordered_map<std::string, ID3D11InputLayout*> m_input_layouts;

	// TODO: Lookup shader file path ref.
};
