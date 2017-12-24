#include "stdafx.h"
#include "app_manager.h"
#include "PlayerDlg.h"

const DWORD PLAY_TIMER = ::RegisterWindowMessageW(L"WG_FORGE_PLAY_TIMER");

PlayerDlg::PlayerDlg(AppManager::GameController* pController)
	: m_pController(pController)
	, m_bMouseCaptured(false)
{
	
}


PlayerDlg::~PlayerDlg()
{
}

void PlayerDlg::maxTurn(int val)
{
	m_nMaxTurn = val;
	m_tracker.Attach(GetDlgItem(IDC_SLIDER));
	m_tracker.SetRangeMin(0);	
	m_tracker.SetTicFreq(1);
	m_tracker.SetRangeMax(m_nMaxTurn, TRUE);
}

void PlayerDlg::tick(float deltaTime)
{
	m_curTime += deltaTime;
	if (!m_bPause)
	{
		if ((m_curTime - m_turnTime) > 1)
		{ // make turn
			int make_turns = int(m_curTime - m_turnTime);
			m_pController->turn(m_pController->turn() + make_turns);
			m_turnTime = m_curTime;
			m_tracker.SetPos(m_pController->turn());
		}		
	}
	else
	{
		m_turnTime = m_curTime;
	}
	
}

bool PlayerDlg::isPaused() const
{
	return m_bPause;
}

float PlayerDlg::turnTime()
{
	return m_turnTime;
}

LRESULT PlayerDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//CenterWindow(GetParent());
	CRect parentRect;
	::GetClientRect(GetParent(), &parentRect);
	
	CRect wndRect;
	GetWindowRect(&wndRect);
	CPoint leftBottom(parentRect.left, parentRect.bottom);
	::ClientToScreen(GetParent(), &leftBottom);
	wndRect.MoveToX(leftBottom.x);
	wndRect.MoveToY(leftBottom.y - (wndRect.bottom - wndRect.top));
	return SetWindowPos(HWND_TOPMOST, wndRect, SWP_NOSIZE /*| SWP_NOZORDER*/ | SWP_NOACTIVATE);
}

LRESULT PlayerDlg::OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	this->SetCapture();	
	::GetCursorPos(&m_initPoint);
	m_bMouseCaptured = true;
	return TRUE;
}

LRESULT PlayerDlg::OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	ReleaseCapture();
	m_bMouseCaptured = false;
	return TRUE;
}

LRESULT PlayerDlg::OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	static bool ignoreNextMouseMove = false;
	if (ignoreNextMouseMove)
	{
		ignoreNextMouseMove = false;
	}
	else if (m_bMouseCaptured)
	{
		CPoint curPos;
		::GetCursorPos(&curPos);
		CRect wndRect;
		this->GetWindowRect(wndRect);
		//this->ClientToScreen(wndRect);
		auto deltaX = curPos.x - m_initPoint.x;
		auto deltaY = curPos.y - m_initPoint.y;
		wndRect.MoveToXY(wndRect.left + deltaX, wndRect.top + deltaY);
		MoveWindow(wndRect);
		//this->GetWindowRect(wndRect);
		m_initPoint = curPos;
		ignoreNextMouseMove = true;
	}
	
	return TRUE;
}


LRESULT PlayerDlg::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDOK)
	{
		/*****/
	}
	//EndDialog(wID);
	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonPrev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_pController->turn() > 0)
	{
		m_pController->turn(m_pController->turn() - 1);
		m_tracker.SetPos((int)m_pController->turn());
	}
	
	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (m_pController->turn() < m_nMaxTurn)
	{
		m_pController->turn(m_pController->turn() + 1);
		m_tracker.SetPos((int)m_pController->turn());
	}

	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonBegin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_pController->turn(0);
	m_tracker.SetPos((int)m_pController->turn());
	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonEnd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_pController->turn(m_pController->maxTurn());
	m_tracker.SetPos(m_pController->turn());
	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bPause = false;

	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonStop(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bPause = true;

	return 0;
}


LRESULT PlayerDlg::OnBnClickedButtonPause(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	m_bPause = !m_bPause;
	return 0;
}


LRESULT PlayerDlg::OnNMReleasedcaptureSlider(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
	int pos = m_tracker.GetPos();
	m_pController->turn(pos);
	return 0;
}
