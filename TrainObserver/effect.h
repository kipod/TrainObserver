#pragma once
#include <d3dx9effect.h>
#include <vector>
#include "defs.hpp"
#include <memory>


class IEffectProperty
{
public:
	virtual bool setProperty(LPD3DXEFFECT pEffect) const = 0;
};


class Effect
{
public:
	~Effect();

	bool begin();
	void end();
	bool beginPass(uint i);
	void endPass();

	static Effect* create(LPDIRECT3DDEVICE9 pDevice, const std::string& path);

	template<class T>
	void addProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value);

private:
	Effect();

	bool setProperties();
private:
	LPD3DXEFFECT	m_effect = nullptr;
	D3DXHANDLE		m_technique = nullptr;
	uint			m_nPasses;
	
	std::vector< std::unique_ptr<IEffectProperty> > m_properties;

	bool			m_hasBegun = false;
	bool			m_hasBegunPass = false;
public:
	uint numPasses() const;
};


template<typename T>
class SimpleEffectProperty : public IEffectProperty
{

public:
	SimpleEffectProperty(const char* name);
	SimpleEffectProperty(const char* name, const T& value);

	virtual bool setProperty(LPD3DXEFFECT pEffect) const;
protected:
	T			m_value;
	std::string	m_name;
};

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name) :
	m_name(name)
{
}

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name, const T& value) :
	m_name(name),
	m_value(value)
{
}

class TextureEffectProperty : public SimpleEffectProperty<LPDIRECT3DTEXTURE9>
{
public:
	TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value);
	TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path);
};

template<class T>
void Effect::addProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value)
{
	m_properties.emplace_back(new SimpleEffectProperty<T>(name, value));
}

