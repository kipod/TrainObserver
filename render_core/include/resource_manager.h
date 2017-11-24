#pragma once
#include <unordered_map>
#include <memory>

template<class T>
class ResourceManager
{
public:

	T* get(const std::string& path);

private:
	std::unordered_map<std::string, std::unique_ptr<T> > m_resources;
};

template<class T>
T* ResourceManager<T>::get(const std::string& path)
{
	auto found = m_resources.find(path);

	if (found != m_resources.end())
	{
		return found->second.get();
	}


	T* newResource = T::create(path);

	if (newResource)
	{
		m_resources.insert(std::make_pair(path, newResource));
	}

	return newResource;
}