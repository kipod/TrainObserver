#include "window_manager.h"
#include <windows.h>
#include <tchar.h>
#include "math\matrix.h"
#include "message_interface.h"

struct WindowImpl
{
	WNDCLASSEX	winClass;
	HWND		hwnd;
};

WindowManager::WindowManager():
	m_impl(new WindowImpl())
{
	s_pInstance = this;
}


WindowManager::~WindowManager()
{
	s_pInstance = nullptr;
}

int lastx = 0;
int lasty = 0;
void WindowManager::onMouseMove(int x, int y, bool bLeftButton)
{
	static bool firstTime = true;
	if (firstTime)
	{
		lastx = x;
		lasty = y;
		firstTime = false;
	}

	if (!s_pInstance)
		return;

	int delta_x = x - lastx;
	int delta_y = y - lasty;
	lastx = x;
	lasty = y;

	for (auto& listener : s_pInstance->m_inputListeners)
	{
		listener->onMouseMove(x, y, delta_x, delta_y, bLeftButton);
	}
}

void WindowManager::onMouseWheel(int nMouseWheelDelta)
{
	if (!s_pInstance)
		return;

	for (auto& listener : s_pInstance->m_inputListeners)
	{
		listener->onMouseWheel(nMouseWheelDelta);
	}
}


void WindowManager::addInputListener(IInputListener* pListener)
{
	m_inputListeners.emplace_back(pListener);
}

void WindowManager::addTickListener(class ITickable* pListener)
{
	m_tickListeners.emplace_back(pListener);
}

HWND WindowManager::hwnd() const
{
	return m_impl->hwnd;
}

//-----------------------------------------------------------------------------
// Name: WindowProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT CALLBACK WindowProc(HWND   hWnd,
	UINT   msg,
	WPARAM wParam,
	LPARAM lParam)
{
	int xPos = (short)LOWORD(lParam);
	int yPos = (short)HIWORD(lParam);
	int nMouseWheelDelta = (short)HIWORD(wParam);
	int nMouseButtonState = LOWORD(wParam);
	bool bLeftButton = ((nMouseButtonState & MK_LBUTTON) != 0);

	switch (msg)
	{
	case WM_KEYDOWN:
	{
		switch (wParam)
		{
		case VK_ESCAPE:
			PostQuitMessage(0);
			break;

		}
	}
	break;

	case WM_CLOSE:
	{
		PostQuitMessage(0);
	}

	case WM_MOUSEMOVE:
	{
		WindowManager::onMouseMove(xPos, yPos, bLeftButton);
	}
	break;
	case WM_MOUSEWHEEL:
	{
		WindowManager::onMouseWheel(nMouseWheelDelta);
	}
	break;
	case WM_DESTROY:
	{
		PostQuitMessage(0);
	}
	break;

	default:
	{
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	break;
	}

	return 0;
}


HRESULT WindowManager::create(HINSTANCE hInstance, int nCmdShow, uint width, uint height)
{
	m_impl->winClass.lpszClassName = _T("MY_WINDOWS_CLASS");
	m_impl->winClass.cbSize = sizeof(WNDCLASSEX);
	m_impl->winClass.style = CS_HREDRAW | CS_VREDRAW;
	m_impl->winClass.lpfnWndProc = WindowProc;
	m_impl->winClass.hInstance = hInstance;
	m_impl->winClass.hIcon = LoadIcon(hInstance, (LPCTSTR)_T("icon.png"));
	m_impl->winClass.hIconSm = LoadIcon(hInstance, (LPCTSTR)_T("icon.png"));
	m_impl->winClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	m_impl->winClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	m_impl->winClass.lpszMenuName = NULL;
	m_impl->winClass.cbClsExtra = 0;
	m_impl->winClass.cbWndExtra = 0;

	if (!RegisterClassEx(&m_impl->winClass))
		return E_FAIL;

	m_impl->hwnd = CreateWindowEx(NULL, _T("MY_WINDOWS_CLASS"),
		_T("Direct3D 9c"),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		0, 0, width, height, NULL, NULL, hInstance, NULL);

	if (m_impl->hwnd == NULL)
		return E_FAIL;

	ShowWindow(m_impl->hwnd, nCmdShow);
	UpdateWindow(m_impl->hwnd);

	return S_OK;
}

void WindowManager::destroy()
{
	UnregisterClass(_T("MY_WINDOWS_CLASS"), m_impl->winClass.hInstance);
}

uint WindowManager::mainLoop()
{
	MSG        uMsg;
	memset(&uMsg, 0, sizeof(uMsg));
	double curTime;
	m_lastTime = timeGetTime();
	float deltaTime = 0.0f;

	while (uMsg.message != WM_QUIT)
	{
		if (PeekMessage(&uMsg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&uMsg);
			DispatchMessage(&uMsg);
		}
		else
		{
			// Goofy little timer
			curTime = timeGetTime();
			deltaTime = (float)((curTime - m_lastTime) * 0.001);
			m_lastTime = curTime;

			// Process the keyboard presses
			processInput(deltaTime);

			for (auto& listener : s_pInstance->m_tickListeners)
			{
				listener->tick(deltaTime);
			}
		}
	}

	return uMsg.wParam;
}

WindowManager* WindowManager::s_pInstance = nullptr;

void WindowManager::processInput(float deltaTime)
{
	unsigned char keys[256];
	GetKeyboardState(keys);

	for (auto& listener : s_pInstance->m_inputListeners)
	{
		listener->processInput(deltaTime, keys);
	}
}
