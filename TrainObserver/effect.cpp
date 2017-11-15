#include "effect.h"
#include <assert.h>
#include "log.h"
#include "math\Vector4.h"
#include "math\Matrix.h"


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
			result &= property->setProperty(m_effect);
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
		setProperties();
		m_effect->SetTechnique(m_technique);
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


using PChar = char*;

template<>
void Effect::addProperty<PChar>(LPDIRECT3DDEVICE9 pDevice, const char* name, const PChar& value)
{
	m_properties.emplace_back(new TextureEffectProperty(pDevice, name, value));
}

bool SimpleEffectProperty<int>::setProperty(LPD3DXEFFECT pEffect) const
{
	return SUCCEEDED(pEffect->SetInt(m_name.c_str(), m_value));
}

bool SimpleEffectProperty<float>::setProperty(LPD3DXEFFECT pEffect) const
{
	return SUCCEEDED(pEffect->SetFloat(m_name.c_str(), m_value));
}

bool SimpleEffectProperty<graph::Vector4>::setProperty(LPD3DXEFFECT pEffect) const
{
	return SUCCEEDED(pEffect->SetVector(m_name.c_str(), &m_value));
}

bool SimpleEffectProperty<graph::Matrix>::setProperty(LPD3DXEFFECT pEffect) const
{
	return SUCCEEDED(pEffect->SetMatrix(m_name.c_str(), &m_value));
}

bool SimpleEffectProperty<LPDIRECT3DTEXTURE9>::setProperty(LPD3DXEFFECT pEffect) const
{
	return SUCCEEDED(pEffect->SetTexture(m_name.c_str(), m_value));
}

TextureEffectProperty::TextureEffectProperty(const char* name, const LPDIRECT3DTEXTURE9& value):
	SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name, value)
{

}

TextureEffectProperty::TextureEffectProperty(LPDIRECT3DDEVICE9 pDevice, const char* name, const char* path):
	SimpleEffectProperty<LPDIRECT3DTEXTURE9>(name)

{
	auto hRet = D3DXCreateTextureFromFile(pDevice, path, &m_value);
}
