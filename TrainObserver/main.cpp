
#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "connection_manager.h"
#include "log.h"

#include "defs.hpp"
#include "json_query_builder.h"
#include "space.h"

#define PORT			2000              
#define SERVER_ADDR     L"127.0.0.1"     /* localhost */
#define MAXBUF          1024

#define USER_NAME		"Spectator"

int main()
{
	initLog(new ConsoleLog());
	ConnectionManager manager;
	if (!manager.init() || !manager.connect(SERVER_ADDR, PORT))
	{
		manager.reset();
		return 1;
	}

	JSONQueryWriter writer;

	writer.add("name", USER_NAME);
	manager.sendMessage(Action::LOGIN, &writer.flush());
	
	std::string msg;
	Result res = manager.receiveMessage(msg);

	if(res == Result::OKEY)
	{
		LOG(MSG_NORMAL, L"Logged in to server as %s.", _T(USER_NAME));

		Space space;
		if (!space.init(manager))
		{
			LOG(MSG_ERROR, L"Static space creation failed");
			return 2;
		}
	}


	manager.sendMessage(Action::LOGOUT, nullptr);
	res = manager.receiveMessage(msg);
	if (res == Result::OKEY)
	{
		LOG(MSG_NORMAL, L"Logged out from server.");
	}
	return 0;
}