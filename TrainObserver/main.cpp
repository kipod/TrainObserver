
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include <windows.h>

#include "connection_manager.h"
#include "log.h"
#include "defs.hpp"
#include "json_query_builder.h"
#include "space.h"
#include "window_manager.h"
#include "render_dx9.h"
#include "skybox.h"
#include "app_manager.h"

#define PORT			2000              
#define SERVER_ADDR     L"127.0.0.1"     /* localhost */
#define MAXBUF          1024

#define USER_NAME		"Spectator"

int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nCmdShow)
{
	initLog(new ConsoleLog());
	AppManager app;
	uint result = 0;
	if (app.initialize(hInstance, nCmdShow, 1200, 1000))
	{
		if (app.connect(SERVER_ADDR, PORT, USER_NAME) && app.loadStaticSpace())
		{
			result = app.mainLoop();

			app.disconnect();
		}
	}
	else
	{
		LOG(MSG_ERROR, L"Failed to init app");
	}

	app.finalize();

	return result;
}