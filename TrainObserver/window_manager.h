#pragma once
#include "defs.hpp"
#include <memory>
#include <vector>


class WindowManager
{
public:
	WindowManager();
	~WindowManager();

	HRESULT create(HINSTANCE hInstance, int nCmdShow, uint width, uint height);
	void	destroy();
	uint	mainLoop();


	static void	onMouseMove(int x, int y, bool bLeftButton);
	static void	onMouseWheel(int nMouseWheelDelta);

	void addInputListener(class IInputListener* pListener);
	void addTickListener(class ITickable* pListener);

	HWND hwnd() const;
private:
	void processInput(float deltaTime);

private:
	std::unique_ptr<struct WindowImpl>	m_impl;
	std::vector<IInputListener*>		m_inputListeners;
	std::vector<ITickable*>				m_tickListeners;
	double								m_lastTime;


	static WindowManager* s_pInstance;

};

