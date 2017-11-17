#pragma once
#include <d3dx9effect.h>
#include <vector>
#include "defs.hpp"
#include <memory>


class IEffectProperty
{
public:
	IEffectProperty(const std::string& name);

	virtual bool applyProperty(LPD3DXEFFECT pEffect) const = 0;
	const std::string& name() const { return m_name;  }

protected:
	std::string	m_name;
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
	void setProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value);

private:
	Effect();

	inline void setProp(IEffectProperty* newProp);

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

	bool applyProperty(LPD3DXEFFECT pEffect) const;
	bool setValue(const T& value);
protected:
	T			m_value;
};

template<typename T>
bool SimpleEffectProperty<T>::setValue(const T& value)
{
	m_value = value;
}

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name) :
	IEffectProperty(name)
{
}

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name, const T& value) :
	IEffectProperty(name),
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
void Effect::setProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value)
{
	setProp(new SimpleEffectProperty<T>(name, value));
}


