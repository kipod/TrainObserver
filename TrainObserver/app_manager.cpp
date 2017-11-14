#include "app_manager.h"
#include "scene_manager.h"
#include "window_manager.h"
#include "connection_manager.h"
#include "log.h"
#include "json_query_builder.h"
#include <tchar.h>
#include "space.h"



AppManager::AppManager():
	m_sceneManager(new SceneManager()),
	m_windowManager(new WindowManager()),
	m_renderSystem(new RenderSystemDX9()),
	m_connectionManager(new ConnectionManager()),
	m_connected(false)
{
}


AppManager::~AppManager()
{
}

bool AppManager::initialize(HINSTANCE hInstance, int nCmdShow, uint width, uint height)
{
	if ( FAILED( m_windowManager->create(hInstance, nCmdShow, width, height) ) )
	{
		LOG(MSG_ERROR, L"Failed to initialize window manager");
		return false;
	}

	if ( FAILED( m_renderSystem->init(m_windowManager->hwnd()) ) )
	{
		LOG(MSG_ERROR, L"Failed to initialize render system");
		return false;
	}


	if ( FAILED( m_connectionManager->init() ) )
	{
		LOG(MSG_ERROR, L"Failed to initialize connection manager");
		return false;
	}

	if (!m_sceneManager->init(m_renderSystem->renderer()))
	{
		LOG(MSG_ERROR, L"Failed to initialize scene manager");
		return false;
	}

	m_windowManager->addInputListener(&m_renderSystem->renderer());
	
	m_windowManager->addTickListener(this);
	m_windowManager->addTickListener(&m_renderSystem->renderer());
	//m_windowManager->addTickListener(m_sceneManager.get());	


	return true;
}

bool AppManager::connect(const wchar_t* servername, int portNumber, const char* username)
{
	if(m_connected)
	{
		disconnect();
	}

	if (!m_connectionManager->connect(servername, portNumber))
	{
		LOG(MSG_ERROR, L"Connection failed");
		m_connectionManager->reset();
		return false;
	}

	JSONQueryWriter writer;
	writer.add("name", username);

	if (m_connectionManager->sendMessage(Action::LOGIN, &writer.str()))
	{
		std::string msg;
		Result res = m_connectionManager->receiveMessage(msg);
		if (res == Result::OKEY)
		{
			wchar_t buf[MAX_PATH];
			mbstowcs_s(nullptr, buf, username, MAX_PATH);
			LOG(MSG_NORMAL, L"Logged in to server as %s.", buf);
			m_connected = true;
			return true;
		}
	}

	return false;
}

uint AppManager::mainLoop()
{
	return m_windowManager->mainLoop();
}

void AppManager::disconnect()
{
	if (m_connected)
	{
		if (m_connectionManager->sendMessage(Action::LOGOUT, nullptr))
		{
			std::string msg;
			Result res = m_connectionManager->receiveMessage(msg);
			if (res == Result::OKEY)
			{
				LOG(MSG_NORMAL, L"Logged out from server.");
				m_connected = false;
			}
		}
	}
}

void AppManager::finalize()
{
	m_connectionManager->reset();
	m_renderSystem->fini();
	m_windowManager->destroy();
}

bool AppManager::loadStaticSpace()
{
	return m_sceneManager->space().initStaticLayer(*m_connectionManager);
}

void AppManager::tick(float deltaTime)
{
	m_sceneManager->space().updateDynamicLayer(*m_connectionManager);
}
