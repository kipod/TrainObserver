#include "ui_manager.h"
#include "ui.h"
#include <assert.h>


UIManager::UIManager()
{
}


UIManager::~UIManager()
{
	m_dialogResourceManager->OnD3D9DestroyDevice();
}

void UIManager::init(HWND hwnd, LPDIRECT3DDEVICE9 pDevice, uint width, uint height)
{
	m_dialogResourceManager.reset(new CDXUTDialogResourceManager());
	m_dialogResourceManager->OnD3D9CreateDevice(pDevice);
	m_dialogResourceManager->OnD3D9ResetDevice();
	m_view.reset(new CDXUTDialog(hwnd));
	m_view->Init(m_dialogResourceManager.get());
	m_view->SetSize(width, height);
	m_view->SetLocation(0, 0);
}

class CDXUTDialog& UIManager::view()
{
	assert(m_view != nullptr);
	return *m_view.get();
}

void UIManager::draw(class RendererDX9& renderer)
{
	auto time = timeGetTime();

	m_view->OnRender(float(time) / 1000.0f);
}

void UIManager::setCallback(PCALLBACKDXUTGUIEVENT callback)
{
	m_view->SetCallback(callback);
}

LRESULT UIManager::msgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing)
{
	if (!m_view)
	{
		return 0;
	}

	*pbNoFurtherProcessing = m_view->MsgProc(hWnd, uMsg, wParam, lParam);
	if (*pbNoFurtherProcessing)
		return 0;

	return 0;
}

