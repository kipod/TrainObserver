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

	void setInt(LPDIRECT3DDEVICE9 pDevice, const char* name, int value);
	void setBool(LPDIRECT3DDEVICE9 pDevice, const char* name, bool value);
	void setFloat(LPDIRECT3DDEVICE9 pDevice, const char* name, float value);
	void setVector(LPDIRECT3DDEVICE9 pDevice, const char* name, const D3DXVECTOR4& value);
	void setMatrix(LPDIRECT3DDEVICE9 pDevice, const char* name, const D3DXMATRIX& value); 
	void setTexture(LPDIRECT3DDEVICE9 pDevice, const char* name, const LPDIRECT3DTEXTURE9& value);
	void setTexture(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path);

	template<class T>
	void setValue(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value);
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


template<class T>
class ValueProperty : public IEffectProperty
{
public:
	ValueProperty(const char* name, const T& value):
		IEffectProperty(name),
		m_value(value)
	{
	}

	virtual bool applyProperty(LPD3DXEFFECT pEffect) const override
	{
		return SUCCEEDED(pEffect->SetValue(m_name.c_str(), &m_value, sizeof(m_value)));
	}
private:
	T m_value;
};


template<class T>
void Effect::setValue(LPDIRECT3DDEVICE9 pDevice, const char* name, const T& value)
{
	setProp(new ValueProperty<T>(name, value));
}
