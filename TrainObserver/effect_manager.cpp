#include "effect_manager.h"
#include "effect.h"
#include "render_dx9.h"



EffectManager::EffectManager()
{
}


EffectManager::~EffectManager()
{
}

Effect* EffectManager::get(const std::string& path)
{
	auto found = m_effects.find(path);

	if (found != m_effects.end())
	{
		return found->second.get();
	}

	
	Effect* newEffect = Effect::create(RenderSystemDX9::instance().renderer().device(), path);

	if (newEffect)
	{
		m_effects.insert(std::make_pair(path, newEffect));
	}

	return newEffect;
}
