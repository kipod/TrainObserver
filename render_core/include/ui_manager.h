#pragma once
#include "render_interface.h"
#include <memory>
#include <d3d9.h>

typedef struct HWND__ *HWND;
typedef void (__stdcall *PCALLBACKDXUTGUIEVENT)(unsigned int nEvent, int nControlID, class CDXUTControl* pControl, void* pUserContext);

class UIManager : public IRenderable//, public IInputListener
{
public:
	UIManager();
	~UIManager();

	void init(HWND hwnd, LPDIRECT3DDEVICE9 pDevice, uint width, uint height);

	class CDXUTDialog& view();

	virtual void draw(class RendererDX9& renderer) override;


	void setCallback(PCALLBACKDXUTGUIEVENT callback);

	LRESULT msgProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, bool* pbNoFurtherProcessing);


private:
	std::unique_ptr<CDXUTDialog> m_view;
	std::unique_ptr<class CDXUTDialogResourceManager>  m_dialogResourceManager;
};

