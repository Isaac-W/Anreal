// ManageDlg.cpp : implementation of the CManageDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "ManageDlg.h"

CManageDlg::CManageDlg(CString strProfileDir) :
	m_strProfileDir(strProfileDir),
	m_strProfileFile(_T("")),
	m_strProfileName(_T(""))
{
	// TODO: Validate profile dir
}

CManageDlg::CManageDlg(CString strProfileDir, CString strProfileFile) :
	m_strProfileDir(strProfileDir),
	m_strProfileFile(strProfileFile),
	m_strProfileName(_T(""))
{
	// TODO: Validate profile dir
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
	//	Load profile from file
	//

	if (m_strProfileFile.GetLength())
	{
		m_prfConfig.Load(m_strProfileFile);
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
	// Create profile path
	CString strPath = m_strProfileDir;
	strPath.AppendFormat(_T("%s.%s"), m_prfConfig.m_strName, PROFILE_EXT);

	// Save profile to file
	m_prfConfig.Save(strPath);

	EndDialog(wID);
	return 0;
}

LRESULT CManageDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
