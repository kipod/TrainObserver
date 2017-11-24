#include "log_interface.h"
#include "app_manager.h"
#include <windows.h>

#define PORT			2000              
#define SERVER_ADDR     "127.0.0.1"     /* localhost */
#define USER_NAME		"Spectator"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	AppManager app;
	uint result = 0;

	if (app.initialize(hInstance, nCmdShow, 1600, 1100))
	{
		if (app.connect(SERVER_ADDR, PORT, USER_NAME) && app.loadStaticSpace())
		{
			result = app.mainLoop();

			app.disconnect();
		}
	}
	else
	{
		LOG(MSG_ERROR, "Failed to init app");
	}

	app.finalize();

	return result;
}