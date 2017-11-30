#pragma once
#include "resource.h"


class PlayerDlg : public CDialogImpl<PlayerDlg>
{
public:
	enum
	{
		IDD = IDD_PLAYER_DLG
	};

	PlayerDlg(AppManager::GameController*);
	virtual ~PlayerDlg();

	void maxTurn(int val);

protected:
	BEGIN_MSG_MAP(SelectGameDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP, OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSEMOVE, OnMouseMove)
		MESSAGE_HANDLER(WM_TIMER, OnTimer)
		COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
		COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
		COMMAND_HANDLER(IDC_BUTTON_PREV, BN_CLICKED, OnBnClickedButtonPrev)
		COMMAND_HANDLER(IDC_BUTTON_NEXT, BN_CLICKED, OnBnClickedButtonNext)
		COMMAND_HANDLER(IDC_BUTTON_BEGIN, BN_CLICKED, OnBnClickedButtonBegin)
		COMMAND_HANDLER(IDC_BUTTON_END, BN_CLICKED, OnBnClickedButtonEnd)
		COMMAND_HANDLER(IDC_BUTTON_PLAY, BN_CLICKED, OnBnClickedButtonPlay)
	END_MSG_MAP()

	LRESULT		OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnTimer(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT		OnBnClickedButtonPrev(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT		OnBnClickedButtonNext(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	AppManager::GameController* m_pController;
	int m_nMaxTurn;
	CTrackBarCtrl m_tracker;	

	// for move window implement
	CPoint m_initPoint;
	bool m_bMouseCaptured;
public:
	LRESULT OnBnClickedButtonBegin(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonEnd(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnBnClickedButtonPlay(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};

