#include "stdafx.h"
#include "app_manager.h"
#include "scene_manager.h"
#include "window_manager.h"
#include "connection_manager.h"
#include "render_dx9.h"
#include "log.h"
#include "json_query_builder.h"
#include "space.h"
#include "SelectGameDlg.h"



AppManager::AppManager()
	: m_sceneManager(new SceneManager())
	, m_windowManager(new WindowManager())
	, m_renderSystem(new RenderSystemDX9())
	, m_connectionManager(new ConnectionManager())
	, m_connected(false)
{
}


AppManager::~AppManager()
{
}

bool AppManager::initialize(HINSTANCE hInstance, int nCmdShow, uint width, uint height)
{
	if (FAILED(m_windowManager->create(hInstance, nCmdShow, width, height)))
	{
		LOG(MSG_ERROR, "Failed to initialize window manager");
		return false;
	}

	initLog(new WindowLog(m_windowManager->hwnd()));

	if (FAILED(m_renderSystem->init(m_windowManager->hwnd())))
	{
		LOG(MSG_ERROR, "Failed to initialize render system");
		return false;
	}


	if (FAILED(m_connectionManager->init()))
	{
		LOG(MSG_ERROR, "Failed to initialize connection manager");
		return false;
	}

	if (!m_sceneManager->init(m_renderSystem->renderer()))
	{
		LOG(MSG_ERROR, "Failed to initialize scene manager");
		return false;
	}

	m_windowManager->addInputListener(&m_renderSystem->renderer());

	m_windowManager->addTickListener(this);
	m_windowManager->addTickListener(&m_renderSystem->renderer());
	// m_windowManager->addTickListener(m_sceneManager.get());


	return true;
}

bool AppManager::connect(const char* servername, uint16_t portNumber, const char* username)
{
	if (m_connected)
	{
		disconnect();
	}

	if (!m_connectionManager->connect(servername, portNumber))
	{
		LOG(MSG_ERROR, "Connection failed");
		m_connectionManager->reset();
		return false;
	}

	JSONQueryWriter writer;
	writer.add("name", username);

	if (m_connectionManager->sendMessage(Action::OBSERVER, &writer.str()))
	{
		std::string msg;
		Result res = m_connectionManager->receiveMessage(msg);
		if (res == Result::OKEY)
		{
			LOG(MSG_NORMAL, "Logged in to server as %s.", username);
			m_connected = true;

			JSONQueryReader data(msg);
			std::map<uint32_t, std::string> games;
			for (const auto& game : data.asArray())
			{
				std::string name = game.get<std::string>("name");
				uint32_t idx = game.get<unsigned int>("idx");
				games[idx] = name;
			}

			SelectGameDlg dlg(games);
			if (IDOK == dlg.DoModal())
			{
				uint32_t idGame = dlg.getGameID();
				JSONQueryWriter writer;
				writer.add("idx", idGame);
				if (m_connectionManager->sendMessage(Action::GAME, &writer.str()))
				{
					std::string msg;
					Result res = m_connectionManager->receiveMessage(msg);
					if (res == Result::OKEY)
					{
					}
				}
			}

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
				LOG(MSG_NORMAL, "Logged out from server.");
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
	return m_sceneManager->initStaticScene(*m_connectionManager);
}

void AppManager::tick(float deltaTime)
{
	m_sceneManager->space().updateDynamicLayer(*m_connectionManager);
}
