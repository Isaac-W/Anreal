// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"

#include "ConfigDlg.h"
#include "ManageDlg.h"

CMainDlg::CMainDlg() :
	m_hTitleFont(NULL),
	m_nTrackerPort(DEFAULT_TRACKER_PORT),
	m_nDisplayPort(DEFAULT_CAPTURE_PORT),
	m_nCompressionLvl(DEFAULT_COMPRESS_LVL)
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

	ATLASSERT(m_hTitleFont);
	GetDlgItem(IDC_TITLE).SetFont(m_hTitleFont);
	
	//
	//	Init paths
	//

	TCHAR szBuf[_MAX_PATH];
	GetModuleFileName(NULL, szBuf, _MAX_PATH); // Get the executable path

	CString strPath(szBuf);
	strPath = strPath.Left(strPath.ReverseFind('\\')); // Get just the current directory
	m_strAppPath = strPath; // Cache executable directory

	// Make config path
	m_strConfigPath = strPath + CONFIG_PATH;

	// Make default profile path
	m_strProfilePath = strPath + DEFAULT_PROFILE_PATH;

	//
	//	Load config
	//

	HRESULT res = LoadConfig(m_strConfigPath);
	ATLASSERT(SUCCEEDED(res));

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
	dlg.m_strProfilePath = m_strProfilePath;
	dlg.m_nTrackerPort = m_nTrackerPort;
	dlg.m_nDisplayPort = m_nDisplayPort;
	dlg.m_nCompressionLvl = m_nCompressionLvl;

	// Show dialog
	int nRet = dlg.DoModal();

	if (IDOK == nRet)
	{
		// Update settings
		m_strProfilePath = dlg.m_strProfilePath;
		m_nTrackerPort = dlg.m_nTrackerPort;
		m_nDisplayPort = dlg.m_nDisplayPort;
		m_nCompressionLvl = dlg.m_nCompressionLvl;
	}

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
	// TODO: Check if running; prompt to force quit
	
	//
	//	Save settings
	//

	SaveConfig(m_strConfigPath);

	DestroyWindow();
	::PostQuitMessage(nVal);
}

void CMainDlg::RefreshProfiles()
{
	// TODO: Enumerate all profile files in profile path
	// Read name of each profile and put into combobox
}

HRESULT CMainDlg::LoadConfig(CString strPath)
{
	TCHAR szBuf[_MAX_PATH]; // Temporary buffer for reading
	DWORD nRet = 0;

	GetLastError(); // Discard current error

	//
	//	Application settings
	//

	// Profile path
	nRet = GetPrivateProfileString(_T("Application"), _T("profile_path"), _T(""), szBuf, _MAX_PATH, strPath);

	if (nRet && (ERROR_FILE_NOT_FOUND != GetLastError()))
	{
		m_strProfilePath = szBuf;
	}

	//
	//	Device settings
	//

	// Tracker port
	nRet = GetPrivateProfileString(_T("Device"), _T("tracker_port"), _T("5250"), szBuf, _MAX_PATH, strPath);

	if (nRet && (ERROR_FILE_NOT_FOUND != GetLastError()))
	{
		int iTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 < iTemp)) m_nTrackerPort = iTemp;
	}

	// Display port
	nRet = GetPrivateProfileString(_T("Device"), _T("display_port"), _T("5251"), szBuf, _MAX_PATH, strPath);

	if (nRet && (ERROR_FILE_NOT_FOUND != GetLastError()))
	{
		int iTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 < iTemp)) m_nDisplayPort = iTemp;
	}

	// Compression level
	nRet = GetPrivateProfileString(_T("Device"), _T("compression"), _T("80"), szBuf, _MAX_PATH, strPath);

	if (nRet && (ERROR_FILE_NOT_FOUND != GetLastError()))
	{
		int iTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 <= iTemp)) m_nCompressionLvl = iTemp;
	}

	return S_OK;
}

HRESULT CMainDlg::SaveConfig(CString strPath)
{
	TCHAR szBuf[_MAX_PATH]; // Temporary buffer for writing
	BOOL bSuccess = FALSE;

	//
	//	Application settings
	//

	// Profile path
	bSuccess = WritePrivateProfileString(_T("Application"), _T("profile_path"), m_strProfilePath, strPath);
	ATLASSERT(bSuccess);

	//
	//	Device settings
	//

	// Tracker port
	_stprintf_s(szBuf, _MAX_PATH, _T("%u"), m_nTrackerPort);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("tracker_port"), szBuf, strPath);
	ATLASSERT(bSuccess);

	// Display port
	_stprintf_s(szBuf, _MAX_PATH, _T("%u"), m_nDisplayPort);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("display_port"), szBuf, strPath);
	ATLASSERT(bSuccess);

	// Compression level
	_stprintf_s(szBuf, _MAX_PATH, _T("%u"), m_nCompressionLvl);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("compression"), szBuf, strPath);
	ATLASSERT(bSuccess);

	return S_OK;
}

HRESULT CMainDlg::LoadProfile(CString strPath)
{
	return E_NOTIMPL;
}
