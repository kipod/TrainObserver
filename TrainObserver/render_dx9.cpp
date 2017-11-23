#include "render_dx9.h"
#include <d3dx9.h>
#include "math\Vector3.h"
#include "resource_manager.h"
#include "texture_manager.h"
#include "effect.h"
#include "camera.h"

namespace
{
	const float FAR_PLANE = 1000000.0f;

	const graph::Vector3	g_vecRight(1.0f, 0.0f, 0.0f);
	const graph::Vector3	g_vecUp(0.0f, 1.0f, 0.0f);    // Up Vector


	class CameraViewProjectionEffectProperty : public IEffectProperty
	{

	public:
		CameraViewProjectionEffectProperty(const Camera& camera) :
			IEffectProperty("ViewProjection"),
			m_camera(camera)
		{}

		virtual bool applyProperty(LPD3DXEFFECT pEffect) const override
		{
			return SUCCEEDED(pEffect->SetMatrix(m_name.c_str(), &m_camera.viewProjection()));
		}
	private:
		const Camera& m_camera;
	};
}

RendererDX9::RendererDX9(LPDIRECT3DDEVICE9 pDevice, const D3DPRESENT_PARAMETERS& d3dpp):
	m_pD3DDevice(pDevice)
{
	m_camera.init(0.01f, FAR_PLANE, D3DXToRadian(60.0f), float(d3dpp.BackBufferWidth) / d3dpp.BackBufferHeight);

	RenderSystemDX9::instance().globalEffectProperties().addProperty(PER_FRAME, new CameraViewProjectionEffectProperty(m_camera));
}


RendererDX9::~RendererDX9()
{
}

void RendererDX9::draw()
{
	if (!m_pD3DDevice)
		return;

	SetCursorPos(100, 100);
	ShowCursor(FALSE);

	m_pD3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
		D3DCOLOR_COLORVALUE(0.0f, 0.0f, 0.0f, 1.0f), 1.0f, 0);

	m_pD3DDevice->BeginScene();

	for (auto& object : m_renderQueue)
	{
		object->draw(*this);
	}

	m_pD3DDevice->EndScene();

	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

void RendererDX9::onMouseMove(int x, int y, int delta_x, int delta_y, bool bLeftButton)
{
	D3DXMATRIX matXRotation;
	D3DXMATRIX matYRotation;
	D3DXMATRIX matRotation;

	// Rotate "camera"
	D3DXMatrixRotationAxis(&matXRotation, &m_camera.up(), float(delta_x) * 0.002f);
	D3DXMatrixRotationAxis(&matYRotation, &(m_camera.up() * m_camera.look()), float(delta_y) * 0.002f);
	D3DXMatrixMultiply(&matRotation, &matXRotation, &matYRotation);

	Vector3 look;
	D3DXVec3TransformNormal(&look, &m_camera.look(), &matRotation);

	m_camera.look(look);
}

float fovDelta = 1.0f;
void RendererDX9::onMouseWheel(int nMouseWheelDelta)
{
	fovDelta *= (1.0f - float(nMouseWheelDelta)*0.0002f);
	m_camera.fov(D3DXToRadian(102.0f*fovDelta));
}

void RendererDX9::tick(float deltaTime)
{
	draw();
}

void RendererDX9::processInput(float deltaTime, unsigned char keys[256])
{
	Vector3 pos = m_camera.pos();

	Vector3 right;
	D3DXVec3Cross(&right, &m_camera.up(), &m_camera.look());

	float multiplier = 1.0f;

	if (keys[VK_LSHIFT] & 0x80)
	{
		multiplier = 30.0f;
	}
	if (keys[VK_LCONTROL] & 0x80)
	{
		multiplier = 0.1f;
	}

	// Left
	if (keys['A'] & 0x80)
	{
		pos += -right * deltaTime * multiplier;
	}

	// Right
	if (keys['D'] & 0x80)
	{
		pos += right * deltaTime * multiplier;
	}

	// Up
	if (keys['W'] & 0x80)
	{
		pos += m_camera.look() * deltaTime * multiplier;
	}

	// Down
	if (keys['S'] & 0x80)
	{
		pos += -m_camera.look() * deltaTime * multiplier;
	}

	// Down
	if (keys['E'] & 0x80)
	{
		pos += graph::Vector3(0.0f, deltaTime*multiplier, 0.0f);
	}

	// Up
	if (keys['Q'] & 0x80)
	{
		pos -= graph::Vector3(0.0f, deltaTime*multiplier, 0.0f);
	}


	m_camera.pos(pos);

	// ---------------
	// Move the teapot
	//if (keys[VK_LEFT] & 0x80)	g_fYaw += 60.0f * fElapsedTime;
	//if (keys[VK_RIGHT] & 0x80)	g_fYaw -= 60.0f * fElapsedTime;
	//if (keys[VK_UP] & 0x80)	g_fPitch += 60.0f * fElapsedTime;
	//if (keys[VK_DOWN] & 0x80)	g_fPitch -= 60.0f * fElapsedTime;

}

LPDIRECT3DDEVICE9 RendererDX9::device()
{
	return m_pD3DDevice;
}


void RendererDX9::addRenderItem(IRenderable* obj)
{
	m_renderQueue.emplace_back(obj);
}

RenderSystemDX9& RenderSystemDX9::instance()
{
	assert(s_pInstance != nullptr);

	return *s_pInstance;
}

RenderSystemDX9::RenderSystemDX9():
	m_effectManager(new EffectManager()),
	m_geometryManager(new GeometryManager()),
	m_textureManager(new TextureManager()),
	m_globalEffectProperties(new EffectConstantManager())
{
	s_pInstance = this;
}

RenderSystemDX9::~RenderSystemDX9()
{
	s_pInstance = nullptr;
}

HRESULT RenderSystemDX9::init(HWND hwnd)
{
	HRESULT hr;
	//if (FAILED(hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_SPEED_OVER_MEMORY)))
	//{
	//	return hr;
	//}


	// Create a direct 3D interface object
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if (m_pD3D == NULL)
	{
		return E_FAIL;
	}

	D3DDISPLAYMODE d3ddm;

	if (FAILED(hr = m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		// TO DO: Respond to failure of GetAdapterDisplayMode
		return hr;
	}

	// 
	if (FAILED(hr = m_pD3D->CheckDeviceFormat(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL,
		d3ddm.Format, D3DUSAGE_DEPTHSTENCIL,
		D3DRTYPE_SURFACE, D3DFMT_D16)))
	{
		if (hr == D3DERR_NOTAVAILABLE)
			// POTENTIAL PROBLEM: We need at least a 16-bit z-buffer!
			return hr;
	}

	//
	// Do we support hardware vertex processing? if so, use it. 
	// If not, downgrade to software.
	//

	D3DCAPS9 d3dCaps;

	if (FAILED(hr = m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,
		D3DDEVTYPE_HAL, &d3dCaps)))
	{
		// TO DO: Respond to failure of GetDeviceCaps
		return hr;
	}

	DWORD dwBehaviorFlags = 0;

	if (d3dCaps.VertexProcessingCaps != 0)
		dwBehaviorFlags |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		dwBehaviorFlags |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	memset(&m_d3dpp, 0, sizeof(m_d3dpp));

	m_d3dpp.BackBufferFormat = d3ddm.Format;
	m_d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed = TRUE;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	// Attempt to create a HAL device, end app on failure just to keep things
	// simple.  In other words we are not trying to create a REF device if the
	// HAL fails.
	if (FAILED(hr = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hwnd,
		dwBehaviorFlags, &m_d3dpp, &m_pD3DDevice)))
	{
		// TO DO: Respond to failure of CreateDevice
		return hr;
	}

	m_renderer.reset(new RendererDX9(m_pD3DDevice, m_d3dpp));

	return S_OK;
}

void RenderSystemDX9::fini()
{
	if (m_pD3DDevice != nullptr)
	{
		m_pD3DDevice->Release();
		m_pD3DDevice = nullptr;
	}

	if (m_pD3D != nullptr)
	{
		m_pD3D->Release();
		m_pD3D = nullptr;
	}

	//CoUninitialize();
}

RenderSystemDX9* RenderSystemDX9::s_pInstance = nullptr;

RendererDX9& RenderSystemDX9::renderer()
{
	return *m_renderer.get();
}

EffectManager& RenderSystemDX9::effectManager()
{
	return *m_effectManager.get();
}

GeometryManager& RenderSystemDX9::geometryManager()
{
	return *m_geometryManager.get();
}

TextureManager& RenderSystemDX9::textureManager()
{
	return *m_textureManager.get();
}

EffectConstantManager& RenderSystemDX9::globalEffectProperties()
{
	return *m_globalEffectProperties.get();
}
