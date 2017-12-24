#pragma once
#include <memory>
#include "defs.hpp"
#include "message_interface.h"


class AppManager : public ITickable
{
public:
	AppManager();
	virtual ~AppManager();
	AppManager(const AppManager&) = delete;
	AppManager& operator=(const AppManager&) = delete;
	AppManager(AppManager&&) = delete;

	struct GameController : public ITickable
	{
		GameController(AppManager* pManager, class ConnectionManager* pConnection);

		virtual ~GameController();

		void initialize();
		void finalize();

		void  turn(int turnNumber);
		int turn() const
		{
			return m_currentTurn;
		}
		int maxTurn() const
		{
			return m_nMaxTurn;
		}
		void maxTurn(int val);

	protected:
		void tick(float deltaTime) override;

	private:
		AppManager*						 m_pAppManager;
		ConnectionManager*				 m_pConnection;
		int								 m_currentTurn = 0;
		int								 m_nMaxTurn = 0;
		std::unique_ptr<class PlayerDlg> m_dlg;
	};

	bool initialize(HINSTANCE hInstance, int nCmdShow, uint width, uint height);
	bool connect(const char* servername, uint16_t portNumber);
	uint mainLoop();
	void disconnect();
	void finalize();
	bool loadStaticSpace();

	void tick(float deltaTime) override;

private:
	std::unique_ptr<class WindowManager>	 m_windowManager;
	std::unique_ptr<class RenderSystemDX9>   m_renderSystem;
	std::unique_ptr<class ConnectionManager> m_connectionManager;
	std::unique_ptr<class SceneManager>		 m_sceneManager;

	bool		   m_connected;
	GameController m_gameController;
};
