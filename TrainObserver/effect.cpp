#include "effect.h"
#include <assert.h>
#include "log.h"
#include "math\Vector4.h"
#include "math\Matrix.h"
#include "texture_manager.h"
#include "render_dx9.h"


namespace
{
	HRESULT makeEffect(LPDIRECT3DDEVICE9 pDevice, LPD3DXEFFECT& pEffect, D3DXHANDLE& hTechnique, const char* path)
	{
		HRESULT hr;
		ID3DXBuffer* errorBuffer = NULL;

		//Load the effect file
		if (FAILED(hr = D3DXCreateEffectFromFile(
			pDevice,
			path,
			NULL,
			NULL,
			0,
			NULL,
			&pEffect,
			&errorBuffer)))
		{
			if (errorBuffer)
			{
#ifdef UNICODE
				WCHAR buffer[10000];
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
	bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const graph::Vector4& value)
	{
		return SUCCEEDED(pEffect->SetVector(name, &value));
	}

	template<>
	bool applyEffectProperty(LPD3DXEFFECT pEffect, const char* name, const graph::Matrix& value)
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

		bool setValue(const T& value)
		{
			m_value = value;
		}
	protected:
		T			m_value;
	};

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
}


Effect::Effect()
{
}


bool Effect::setProperties()
{
	bool result = false;
	if (m_effect)
	{
		result = true;
		for (auto& property : m_properties)
		{
			result &= property->applyProperty(m_effect);
		}
	}

	return result;
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
		setProperties();
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

Effect* Effect::create(LPDIRECT3DDEVICE9 pDevice, const std::string& path)
{
	Effect* pEffect = new Effect();

	if (SUCCEEDED(makeEffect(pDevice, pEffect->m_effect, pEffect->m_technique, path.c_str())))
	{
		return pEffect;
	}

	delete pEffect;
	return nullptr;
}


void Effect::setInt(LPDIRECT3DDEVICE9 pDevice, const char* name, int value)
{
	setProp(new SimpleEffectProperty<int>(name, value));
}

void Effect::setBool(LPDIRECT3DDEVICE9 pDevice, const char* name, bool value)
{
	setProp(new SimpleEffectProperty<bool>(name, value));
}

void Effect::setFloat(LPDIRECT3DDEVICE9 pDevice, const char* name, float value)
{
	setProp(new SimpleEffectProperty<float>(name, value));
}

void Effect::setVector(LPDIRECT3DDEVICE9 pDevice, const char* name, const D3DXVECTOR4& value)
{
	setProp(new SimpleEffectProperty<D3DXVECTOR4>(name, value));
}

void Effect::setMatrix(LPDIRECT3DDEVICE9 pDevice, const char* name, const D3DXMATRIX& value)
{
	setProp(new SimpleEffectProperty<D3DXMATRIX>(name, value));
}

void Effect::setTexture(LPDIRECT3DDEVICE9 pDevice, const char* name, const LPDIRECT3DTEXTURE9& value)
{
	setProp(new SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, value));
}

void Effect::setTexture(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path)
{
	Texture* pTex = RenderSystemDX9::instance().textureManager().get(path);
	if (pTex)
	{
		setProp(new SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, pTex));
	}
}

using PChar = char*;

void Effect::setProp(IEffectProperty* newProp)
{
	for (auto& prop : m_properties)
	{
		if (prop->name() == newProp->name())
		{
			prop.reset(newProp);
			return;
		}
	}

	m_properties.emplace_back(newProp);
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////

class TextureEffectProperty : public SimpleEffectProperty<LPDIRECT3DTEXTURE9>
{
public:
	TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value);
	TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path);
};


TextureEffectProperty::TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value):
	SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, value)
{

}

TextureEffectProperty::TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path):
	SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name)

{
	auto hRet = D3DXCreateTextureFromFile(pDevice, path, &m_value);
}

IEffectProperty::IEffectProperty(const std::string& name):
	m_name(name)
{

}

