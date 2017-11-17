#pragma once
#include <unordered_map>
#include <memory>

class EffectManager
{
public:
	EffectManager();
	~EffectManager();

	class Effect* get(const std::string& path);

private:
	std::unordered_map<std::string, std::unique_ptr<Effect> > m_effects;
};

