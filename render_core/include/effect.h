#pragma once
#include <d3dx9effect.h>
#include <vector>
#include <memory>
#include "render_interface.h"


class IEffectProperty
{
public:
	virtual ~IEffectProperty() {}
	IEffectProperty(const std::string& name);

	virtual bool applyProperty(LPD3DXEFFECT pEffect) const = 0;
	virtual void update() {}
	const std::string& name() const { return m_name;  }

protected:
	std::string	m_name;
};

class EffectProperties
{
public:
	~EffectProperties();

	void setInt(const char* name, int value);
	void setBool(const char* name, bool value);
	void setFloat(const char* name, float value);
	void setVector(const char* name, const D3DXVECTOR4& value);
	void setMatrix(const char* name, const D3DXMATRIX& value); 
	void setTexture(const char* name, const LPDIRECT3DTEXTURE9& value);
	void setTexture(const char* name, const char* path);

	template<class T>
	void setValue(const char* name, const T& value);

	void addProperty(IEffectProperty*);
	void updateProperties();

	bool applyProperties(class Effect* pEffect) const;
private:

	inline void setProp(IEffectProperty* newProp);
	inline IEffectProperty* findProp(const char* name);

private:
	std::vector< std::shared_ptr<IEffectProperty> > m_properties;
};

enum EConstantType
{
	GLOBAL,
	PER_FRAME,
	EFFECT_CONSTANT_TYPE_COUNT
};

class EffectConstantManager
{
public:
	void addProperty(EConstantType type, IEffectProperty* property);
	void update(EConstantType type);
	void applyProperties(Effect* pEffect);
private:
	EffectProperties m_properties[EFFECT_CONSTANT_TYPE_COUNT];
};

class Effect
{
public:
	~Effect();

	bool begin();
	void end();
	bool beginPass(uint i);
	void endPass();
	unsigned int numPasses() const;

	static Effect* create(const std::string& path);

	void flush();
private:
	void applyGlobalProperties();
	Effect();
private:
	LPD3DXEFFECT	m_effect = nullptr;
	D3DXHANDLE		m_technique = nullptr;
	unsigned int 	m_nPasses;

	bool			m_hasBegun = false;
	bool			m_hasBegunPass = false;

	friend class EffectProperties;
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
void EffectProperties::setValue(const char* name, const T& value)
{
	setProp(new ValueProperty<T>(name, value));
}
