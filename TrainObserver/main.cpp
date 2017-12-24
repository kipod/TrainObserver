#include "stdafx.h"

#include "log.h"
#include "app_manager.h"
#include "connection_dlg.h"

const size_t MAXBUF = 1024;

CAppModule _Module;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES); // add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));
	ConnectionDialog dlg;
	uint result = 0;
	if (IDOK == dlg.DoModal())
	{
		AppManager app;
		if (app.initialize(hInstance, nCmdShow, 1400, 900))
		{
			if (app.connect(dlg.serverAddr(), dlg.port()))
			{
				if (app.loadStaticSpace())
				{
					result = app.mainLoop();
					app.disconnect();
				}
			}
		}
		else
		{
			LOG(MSG_ERROR, "Failed to init app");
		}
		app.finalize();
	}

	_Module.Term();
	::CoUninitialize();

	return result;
}
