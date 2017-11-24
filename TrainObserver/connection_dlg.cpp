#include "stdafx.h"
#include "resource.h"

#include "connection_dlg.h"
#include <atlmisc.h>

#ifdef DEBUG
const uint16_t PORT = 2000;
const char*	SERVER_ADDR = "127.0.0.1"; /* localhost */
#else
const uint16_t PORT = 443;
const char*	SERVER_ADDR = "wgforge-srv.wargaming.net"; /* official server */
#endif // DEBUG

const char* USER_NAME = "Spectator";

ConnectionDialog::ConnectionDialog()
	: m_serverAddr(SERVER_ADDR)
	, m_port(PORT)
	, m_userName(USER_NAME)
{
}

ConnectionDialog::~ConnectionDialog()
{
}

LRESULT ConnectionDialog::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CenterWindow(GetParent());
	m_cmbSuccessConn.Attach(GetDlgItem(IDC_COMBO_SUCCESS));
	updateSuccessConnections();
	CEdit edit;
	edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_USER_NAME));
	edit.SetWindowTextA(m_userName.c_str());
	return TRUE;
}

LRESULT ConnectionDialog::OnCloseCmd(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	if (wID == IDOK)
	{
		{
			CEdit edit;
			edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_ADDR));
			CComBSTR bstr;
			edit.GetWindowText(&bstr);
			WTL::CString str(bstr);
			m_serverAddr = str;
		}
		{
			CEdit edit;
			edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_PORT));
			CComBSTR bstr;
			edit.GetWindowText(&bstr);
			WTL::CString str(bstr);
			m_port = uint16_t(_ttoi(str));
		}
		{
			CEdit edit;
			edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_USER_NAME));
			CComBSTR bstr;
			edit.GetWindowText(&bstr);
			WTL::CString str(bstr);
			m_userName = str;
		}
	}
	EndDialog(wID);
	return 0;
}

const char* ConnectionDialog::serverAddr()
{
	return m_serverAddr.c_str();
}

uint16_t ConnectionDialog::port()
{
	return m_port;
}

const char* ConnectionDialog::userName()
{
	return m_userName.c_str();
}

void ConnectionDialog::updateSuccessConnections()
{
	m_setSuccessConnections.push_back(std::pair<std::string, uint16_t>("127.0.0.1", 2000));
	m_setSuccessConnections.push_back(std::pair<std::string, uint16_t>("wgforge-srv.wargaming.net", 443));
	m_cmbSuccessConn.ResetContent();
	for (ConnectionData& conn : m_setSuccessConnections)
	{
		WTL::CString str;
		str.Format(TEXT("%s:%d"), conn.first.c_str(), conn.second);
		m_cmbSuccessConn.SetItemData(m_cmbSuccessConn.AddString(str), DWORD_PTR(&conn));
	}
	if (m_cmbSuccessConn.GetCount() > 1)
	{
		m_cmbSuccessConn.SetCurSel(0);
		updateDataFields();
	}
}

void ConnectionDialog::updateDataFields()
{
	int idx = m_cmbSuccessConn.GetCurSel();
	if (idx != CB_ERR)
	{
		auto* conn = reinterpret_cast<ConnectionData*>(m_cmbSuccessConn.GetItemData(idx));
		m_serverAddr = conn->first;
		m_port = conn->second;
		CEdit edit;
		edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_ADDR));
		WTL::CString str;
		// 		CComBSTR bstr;
		// 		edit.GetWindowText(&bstr);
		// 		m_
		edit.SetWindowTextA(conn->first.c_str());
		edit.Detach();
		edit.Attach(GetDlgItem(IDC_EDIT_CONNECT_PORT));
		str.Format(TEXT("%d"), m_port);
		edit.SetWindowTextA(str);
	}
}

LRESULT ConnectionDialog::OnCbnSelchangeComboSuccess(
	WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	updateDataFields();
	return 0;
}
