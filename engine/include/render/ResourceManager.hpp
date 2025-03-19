#pragma once

#include <d3d11.h>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

template<typename T>
class ResourceManager
{
public:

	ComPtr<T> Get(const std::string& name) {
		auto object = m_cache.find(name);

		if (object != m_cache.end())
		{
			return object->second;
		}

		return nullptr;
	}

	bool Load(const std::string& name, ComPtr<T> object) {

		m_cache[name] = object;

		return true;
	}

	bool Unload(const std::string& name) {
		m_cache.erase(name);

		return true;
	}


private:
	std::unordered_map<std::string, ComPtr<T>> m_cache;
};
