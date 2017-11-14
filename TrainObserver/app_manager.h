#pragma once
#include <memory>
#include "defs.hpp"
#include "render_interface.h"



class AppManager: public ITickable
{
public:
	AppManager();
	~AppManager();

	bool initialize(HINSTANCE hInstance, int nCmdShow, uint width, uint height);
	bool connect(const wchar_t* servername, int portNumber, const char* username);
	uint mainLoop();
	void disconnect();
	void finalize();

	bool loadStaticSpace();


	virtual void tick(float deltaTime) override;

private:

private:
	std::unique_ptr<class SceneManager>			m_sceneManager;
	std::unique_ptr<class ConnectionManager>	m_connectionManager;
	std::unique_ptr<class WindowManager>		m_windowManager;
	std::unique_ptr<class RenderSystemDX9>		m_renderSystem;

	bool m_connected;
};

