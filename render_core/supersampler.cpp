#include "supersampler.h"
#include "quad.h"
#include "render_target.h"
#include "render_dx9.h"

static const char* EFFECT_PATH = "content/shaders/bb_copy.fx";

Supersampler::Supersampler(LPDIRECT3DDEVICE9 pD3DDevice, int width, int height, D3DFORMAT pixelFormat):
	m_quad(new Quad()),
	m_backBuffer(new RenderTarget(pD3DDevice, width*2, height*2, pixelFormat))
{
	m_effect = RenderSystemDX9::instance().effectManager().get(EFFECT_PATH);
	m_quad->create(pD3DDevice);
	m_props.setTexture("bbTex", m_backBuffer->pTexture());

}


Supersampler::~Supersampler()
{
}

void Supersampler::draw(LPDIRECT3DDEVICE9 pDevice)
{
	m_props.applyProperties(m_effect);
	m_quad->draw(pDevice, *m_effect);
}

void Supersampler::push()
{
	m_backBuffer->push((RenderType)(RT_COLOR | RT_Z));
}

void Supersampler::pop(LPDIRECT3DDEVICE9 pDevice)
{
	m_backBuffer->pop();

	draw(pDevice);
}
