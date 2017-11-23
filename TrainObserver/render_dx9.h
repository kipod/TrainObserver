#pragma once
#include "render_interface.h"
#include <memory>
#include <d3dx9.h>
#include "math\Matrix.h"
#include "math\Vector3.h"
#include "window_manager.h"
#include "camera.h"
#include "resource_manager.h"
#include "effect.h"
#include "geometry.h"

using namespace graph;

typedef ResourceManager<Effect> EffectManager;
typedef ResourceManager<Geometry> GeometryManager;

class RendererDX9 : public IRenderer, public ITickable
{

public:
	RendererDX9(LPDIRECT3DDEVICE9 pDevice, const D3DPRESENT_PARAMETERS& d3dpp);
	~RendererDX9();

	virtual void draw();

	// IInputListener
	virtual void onMouseMove(int x, int y, int delta_x, int delta_y, bool bLeftButton) override;
	virtual void onMouseWheel(int nMouseWheelDelta) override;
	virtual void tick(float deltaTime) override;
	virtual void processInput(float deltaTime, unsigned char keys[256]) override;

	LPDIRECT3DDEVICE9 device();

	void addRenderItem(IRenderable* obj);


private:
	LPDIRECT3DDEVICE9						m_pD3DDevice = nullptr;

	std::vector<IRenderable*>				m_renderQueue;
};


class RenderSystemDX9 : public IRenderSystem
{

public:
	RenderSystemDX9();
	~RenderSystemDX9();

	virtual HRESULT init(HWND hwnd) override;
	virtual void fini() override;

	RendererDX9& renderer();
	EffectManager& effectManager();
	GeometryManager& geometryManager();
	class TextureManager& textureManager();
	class EffectConstantManager& globalEffectProperties();

	static RenderSystemDX9& instance();


private:
	IDirect3D9*								m_pD3D = nullptr;
	LPDIRECT3DDEVICE9						m_pD3DDevice = nullptr;
	D3DPRESENT_PARAMETERS					m_d3dpp;

	std::unique_ptr<RendererDX9>			m_renderer;
	std::unique_ptr<EffectManager>			m_effectManager;
	std::unique_ptr<GeometryManager>		m_geometryManager;
	std::unique_ptr<TextureManager>			m_textureManager;
	std::unique_ptr<EffectConstantManager>	m_globalEffectProperties;
	static RenderSystemDX9*					s_pInstance;
};


