// ManageDlg.cpp : implementation of the CManageDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "ManageDlg.h"

CManageDlg::CManageDlg() :
	m_strPath(_T("")),
	m_strProfileName(_T(""))
{
}

CManageDlg::CManageDlg(CString strPath) :
	m_strPath(strPath),
	m_strProfileName(_T(""))
{
}

CManageDlg::~CManageDlg()
{
}

LRESULT CManageDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
	//	Load profile
	//

	if (m_strPath.GetLength())
	{
		m_prfConfig.Load(m_strPath);
	}

	//
	//	Init dialog from profile
	//

	// TODO

	return TRUE;
}

LRESULT CManageDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CManageDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Apply();
	EndDialog(wID);
	return 0;
}

LRESULT CManageDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}

LRESULT CManageDlg::OnApply(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	Apply();
	return 0;
}

HRESULT CManageDlg::Apply()
{
	// Delete old file if name changed
	// Save profile config to new file

	return S_OK;
}