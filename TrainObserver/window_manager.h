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


	static void	onMouseMove(int x, int y, int delta_x, int delta_y, bool bLeftButton);
	static void	onMouseWheel(int nMouseWheelDelta);
	static void	onLMouseUp(int x, int y);

	void addInputListener(class IInputListener* pListener);
	void addTickListener(class ITickable* pListener);

	HWND hwnd() const;
private:
	void processInput(float deltaTime);

private:
	std::unique_ptr<struct WindowImpl>	m_impl;
	std::vector<IInputListener*>		m_inputListeners;
	std::vector<ITickable*>				m_tickListeners;

	static WindowManager* s_pInstance;

};

