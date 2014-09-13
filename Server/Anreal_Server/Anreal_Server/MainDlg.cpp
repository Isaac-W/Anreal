// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include "ConfigDlg.h"
#include "ManageDlg.h"

CMainDlg::CMainDlg() :
	m_hTitleFont(NULL)
{
}

CMainDlg::~CMainDlg()
{
}

LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	//
	//	Center dialog
	//

	CenterWindow();

	//
	//	Set icons
	//

	HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	SetIcon(hIcon, TRUE);

	HICON hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(hIconSmall, FALSE);

	//
	//	Set font
	//

	// Title (TODO: Use CreatePointFont?)
	m_hTitleFont = CreateFont(	48, 0,
								0, 0,
								FW_NORMAL,
								FALSE, FALSE, FALSE,
								ANSI_CHARSET,
								OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								ANTIALIASED_QUALITY, DEF_PRIORITY | FF_MODERN,
								"Segoe UI Light");

	_ASSERTE(m_hTitleFont);
	GetDlgItem(IDC_TITLE).SetFont(m_hTitleFont);
	
	//
	//	Load config
	//

	// TODO

	//
	//	Load profiles
	//

	RefreshProfiles();

	//
	//	Select last item in list
	//

	// TODO

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (m_hTitleFont) DeleteObject(m_hTitleFont);

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Toggle thread start/stop-- use transmit helper class (holds settings and thread wrapper)

	// Load settings from file

	// Create tracker/capture objects

	// Set tracker/capture parameters

	// Init tracker/capture

	// Run tracker/capture

	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnConfig(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//
	//	Show Config dialog
	//
	
	// Show configuration dialog; update changes in transmit helper class
	CConfigDlg dlg;

	// Init settings
	// TODO

	// Show dialog
	dlg.DoModal();

	return 0;
}

LRESULT CMainDlg::OnManageProfiles(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//
	//	Get profile name
	//

	// TODO: Store profile name/path in string
	
	//
	//	Show Manage Profiles dialog
	//

	CManageDlg dlg;

	// Init settings
	// TODO

	// Show dialog
	dlg.DoModal();

	//
	//	Update changes
	//

	RefreshProfiles();

	//
	//	Select previous profile
	//

	// TODO: Use name to select profile

	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	//
	//	Save settings
	//

	// TODO

	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::RefreshProfiles()
{

}

HRESULT CMainDlg::LoadConfig(CString strPath)
{
	return E_NOTIMPL;
}

HRESULT CMainDlg::LoadProfile(CString strPath)
{
	return E_NOTIMPL;
}