// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

CMainDlg::CMainDlg() :
	_hTitleFont(NULL)
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

	// Title
	_hTitleFont = CreateFont(	48, 0,
								0, 0,
								FW_NORMAL,
								FALSE, FALSE, FALSE,
								ANSI_CHARSET,
								OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
								ANTIALIASED_QUALITY, DEF_PRIORITY | FF_MODERN,
								"Segoe UI Light");

	_ASSERTE(_hTitleFont);
	GetDlgItem(IDC_TITLE).SetFont(_hTitleFont);
	
	//
	//	Load config
	//

	// TODO

	//
	//	Load profiles
	//

	// TODO

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	if (_hTitleFont) DeleteObject(_hTitleFont);

	return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Toggle thread start/stop-- use transmit helper class (holds settings and thread wrapper)
	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

LRESULT CMainDlg::OnConfig(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Show configuration dialog; update changes in transmit helper class
	return 0;
}

LRESULT CMainDlg::OnManageProfiles(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// Show profile management dialog; update changes in transmit helper class
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}