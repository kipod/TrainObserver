#include "effect.h"
#include <assert.h>
#include "log_interface.h"
#include "math\vector4.h"
#include "math\matrix.h"
#include "texture_manager.h"
#include "render_dx9.h"


namespace EffectInternals
{
HRESULT makeEffect(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT& pEffect, D3DXHANDLE& hTechnique, const char* path)
{
	HRESULT		 hr;
	ID3DXBuffer* errorBuffer = NULL;

	// Load the effect file
	if (FAILED(hr = D3DXCreateEffectFromFile(pDevice, path, NULL, NULL, 0, NULL, &pEffect, &errorBuffer)))
	{
		if (errorBuffer)
		{
#ifdef UNICODE
			char buffer[10000];
			memset(buffer, 0, 10000);
			MultiByteToWideChar(CP_ACP, 0, (char*)errorBuffer->GetBufferPointer(), -1, buffer, 10000);
			LOG(MSG_ERROR, buffer);
#else
			LOG(MSG_ERROR, (char*)errorBuffer->GetBufferPointer());
#endif
			errorBuffer->Release();
		}
		return hr;
	}

	// Find the best technique
	pEffect->FindNextValidTechnique(NULL, &hTechnique);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////

template<class T>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const T& value)
{
	return SUCCEEDED(pEffect->SetValue(name, &value, sizeof(value)));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const bool& value)
{
	return SUCCEEDED(pEffect->SetBool(name, value));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const int& value)
{
	return SUCCEEDED(pEffect->SetInt(name, value));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const float& value)
{
	return SUCCEEDED(pEffect->SetFloat(name, value));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const Vector4& value)
{
	return SUCCEEDED(pEffect->SetVector(name, &value));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const Matrix& value)
{
	return SUCCEEDED(pEffect->SetMatrix(name, &value));
}

template<>
bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const LPDIRECT3DTEXTURE9& value)
{
	return SUCCEEDED(pEffect->SetTexture(name, value));
}


//////////////////////////////////////////////////////////////////////////

template<class T>
class SimpleEffectProperty : public IEffectProperty
{
public:
	SimpleEffectProperty(const char* name);
	SimpleEffectProperty(const char* name, const T& value);

	virtual bool applyProperty(LPD3DXEFFECT pEffect) const override
	{
		return applyEffectProperty(pEffect, m_name.c_str(), m_value);
	}

	void setValue(const T& value)
	{
		m_value = value;
	}

protected:
	T m_value;
};

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name)
	: IEffectProperty(name)
{
}

template<typename T>
SimpleEffectProperty<T>::SimpleEffectProperty(const char* name, const T& value)
	: IEffectProperty(name)
	, m_value(value)
{
}
} // namespace EffectInternals

using namespace EffectInternals;

void Effect::applyGlobalProperties()
{
	RenderSystemDX9::instance().globalEffectProperties().applyProperties(this);
}

Effect::Effect()
{
}

uint Effect::numPasses() const
{
	return m_nPasses;
}


Effect::~Effect()
{
}

bool Effect::begin()
{
	assert(!m_hasBegun);

	if (m_effect && m_technique)
	{
		m_hasBegun = true;
		m_effect->SetTechnique(m_technique);
		applyGlobalProperties();
		return SUCCEEDED(m_effect->Begin(&m_nPasses, 0));
	}
	return false;
}

void Effect::end()
{
	assert(m_hasBegun);

	if (m_effect)
	{
		m_effect->End();
	}

	m_hasBegun = false;
}

bool Effect::beginPass(uint i)
{
	assert(!m_hasBegunPass && m_hasBegun && i < m_nPasses);

	if (m_effect)
	{
		m_hasBegunPass = true;
		return SUCCEEDED(m_effect->BeginPass(i));
	}

	return false;
}

void Effect::endPass()
{
	assert(m_hasBegunPass);

	if (m_effect)
	{
		m_effect->EndPass();
	}

	m_hasBegunPass = false;
}

Effect* Effect::create(const std::string& path)
{
	Effect* pEffect = new Effect();

	if (SUCCEEDED(makeEffect(
			RenderSystemDX9::instance().renderer().device(), pEffect->m_effect, pEffect->m_technique, path.c_str())))
	{
		return pEffect;
	}

	delete pEffect;
	return nullptr;
}


void Effect::flush()
{
	m_effect->CommitChanges();
}

EffectProperties::~EffectProperties()
{
}

#define SET_VALUE(name, value, type)\
	auto prop = findProp(name);	\
	if (prop)\
	{\
		auto simpleProp = dynamic_cast<SimpleEffectProperty<type>*>(prop);\
		if (simpleProp)\
		{\
			simpleProp->setValue(value);\
		}\
	}\
	else\
	{\
		m_properties.emplace_back(new SimpleEffectProperty<type>(name, value));\
	}


void EffectProperties::setInt(const char* name, int value)
{
	SET_VALUE(name, value, int);
}

void EffectProperties::setBool(const char* name, bool value)
{
	SET_VALUE(name, value, bool);
}

void EffectProperties::setFloat(const char* name, float value)
{
	SET_VALUE(name, value, float);
}

void EffectProperties::setVector(const char* name, const D3DXVECTOR4& value)
{
	SET_VALUE(name, value, D3DXVECTOR4);
}

void EffectProperties::setMatrix(const char* name, const D3DXMATRIX& value)
{
	SET_VALUE(name, value, D3DXMATRIX);
}

void EffectProperties::setTexture(const char* name, const LPDIRECT3DTEXTURE9& value)
{
	SET_VALUE(name, value, LPDIRECT3DTEXTURE9);
}

void EffectProperties::setTexture(const char* name, const char* path)
{
	Texture* pTex = RenderSystemDX9::instance().textureManager().get(path);
	if (pTex)
	{
		setProp(new SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, pTex));
	}
}

void EffectProperties::addProperty(IEffectProperty* newProp)
{
	setProp(newProp);
}

void EffectProperties::updateProperties()
{
	for (auto& property : m_properties)
	{
		property->update();
	}
}

bool EffectProperties::applyProperties(Effect* pEffect) const
{
	bool result = false;
	if (pEffect)
	{
		result = true;
		for (const auto& property : m_properties)
		{
			result &= property->applyProperty(pEffect->m_effect);
		}
	}

	return result;
}


void EffectProperties::setProp(IEffectProperty* newProp)
{
	m_properties.emplace_back(newProp);
}

IEffectProperty* EffectProperties::findProp(const char* name)
{
	for (auto& prop : m_properties)
	{
		if (prop->name() == name)
		{
			return prop.get();
		}
	}

	return nullptr;
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class TextureEffectProperty : public SimpleEffectProperty<LPDIRECT3DTEXTURE9>
{
public:
	TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value);
	TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path);
};


TextureEffectProperty::TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value)
	: SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, value)
{
}

TextureEffectProperty::TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path)
	: SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name)

{
	auto hRet = D3DXCreateTextureFromFile(pDevice, path, &m_value);
}

IEffectProperty::IEffectProperty(const std::string& name)
	: m_name(name)
{
}

void EffectConstantManager::addProperty(EConstantType type, IEffectProperty* property)
{
	m_properties[type].addProperty(property);
}

void EffectConstantManager::update(EConstantType type)
{
	m_properties[type].updateProperties();
}

void EffectConstantManager::applyProperties(Effect* pEffect)
{
	for (uint i = 0; i < EFFECT_CONSTANT_TYPE_COUNT; ++i)
	{
		m_properties[i].applyProperties(pEffect);
	}
}
