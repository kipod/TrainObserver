#pragma once

#include <string>
#include <list>
#include "resource.h"

class ConnectionDialog : public CDialogImpl<ConnectionDialog>
{
public:
	enum
	{
		IDD = IDD_CONNECTDLG
	};

	ConnectionDialog();
	~ConnectionDialog();

	BEGIN_MSG_MAP(ConnectionDialog)
	  MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	  COMMAND_ID_HANDLER(IDOK, OnCloseCmd)
	  COMMAND_ID_HANDLER(IDCANCEL, OnCloseCmd)
	  COMMAND_HANDLER(IDC_COMBO_SUCCESS, CBN_SELCHANGE, OnCbnSelchangeComboSuccess)
	END_MSG_MAP()

	// Handler prototypes (uncomment arguments if needed):
	//	LRESULT MessageHandler(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	//	LRESULT CommandHandler(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
	//	LRESULT NotifyHandler(int /*idCtrl*/, LPNMHDR /*pnmh*/, BOOL& /*bHandled*/)

	LRESULT		OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT		OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	const char* serverAddr();
	uint16_t	port();

private:
	std::string m_serverAddr;
	uint16_t	m_port;

	// 	enum class UPDATE_DATA_TYPE : size_t
	// 	{
	// 		SELECTED_SUCCESSED_CMB = 1,
	// 		CHANGED_EDIT_BOX_TEXT = 2
	// 	};

	typedef std::pair<std::string, uint16_t> ConnectionData;
	std::list<ConnectionData>				 m_setSuccessConnections;
	CComboBox								 m_cmbSuccessConn;
	void									 updateSuccessConnections();
	void									 updateDataFields();

public:
	LRESULT OnCbnSelchangeComboSuccess(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
};
