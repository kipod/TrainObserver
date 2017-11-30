#pragma once
#include <memory>
#include <d3dx9.h>

#include "render_interface.h"
#include "camera.h"
#include "resource_manager.h"
#include "effect.h"
#include "geometry.h"
#include "math\matrix.h"
#include "math\vector3.h"
#include "message_interface.h"

typedef ResourceManager<Effect> EffectManager;
typedef ResourceManager<Geometry> GeometryManager;

class RendererDX9: public ITickable, public IInputListener
{

public:
	RendererDX9(LPDIRECT3DDEVICE9 pDevice);
	~RendererDX9();

	void init(const D3DPRESENT_PARAMETERS& d3dpp);

	void draw();

	// IInputListener
	virtual void onMouseMove(int x, int y, int delta_x, int delta_y, bool bLeftButton) override;
	virtual void onMouseWheel(int nMouseWheelDelta) override;
	virtual void tick(float deltaTime) override;
	virtual void processInput(float deltaTime, unsigned char keys[256]) override;

	LPDIRECT3DDEVICE9 device();
	Camera& camera() { return m_camera; }

	void addRenderItem(IRenderable* obj);
	void addPostRenderItem(IRenderable* obj);


private:
	LPDIRECT3DDEVICE9						m_pD3DDevice = nullptr;
	std::unique_ptr<class Supersampler>		m_supersampler;
	Camera									m_camera;

	std::vector<IRenderable*>				m_renderSet;
	std::vector<IRenderable*>				m_postRenderSet;
};


class RenderSystemDX9
{
public:
	RenderSystemDX9();
	~RenderSystemDX9();

	HRESULT init(HWND hwnd);
	void fini();

	RendererDX9& renderer();
	EffectManager& effectManager();
	GeometryManager& geometryManager();
	class TextureManager& textureManager();
	class EffectConstantManager& globalEffectProperties();
	class UIManager& uiManager();

	static RenderSystemDX9& instance();

private:
	IDirect3D9*								m_pD3D = nullptr;
	LPDIRECT3DDEVICE9						m_pD3DDevice = nullptr;
	D3DPRESENT_PARAMETERS					m_d3dpp;

	std::unique_ptr<RendererDX9>			m_renderer;
	std::unique_ptr<EffectManager>			m_effectManager;
	std::unique_ptr<GeometryManager>		m_geometryManager;
	std::unique_ptr<TextureManager>			m_textureManager;
	std::unique_ptr<UIManager>				m_uiManager;
	std::unique_ptr<EffectConstantManager>	m_globalEffectProperties;

	static RenderSystemDX9*					s_pInstance;
};


