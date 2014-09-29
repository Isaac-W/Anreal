// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "ConfigDlg.h"
#include "ManageDlg.h"

#include "Profile.h"

#include "Tracker.h"
#include "NetTracker.h"
#include "MemTrkDriver.h"

CMainDlg::CMainDlg() :
	m_hTitleFont(NULL),
	m_strAppPath(_T("")),
	m_strConfigPath(_T("")),
	m_strProfilePath(_T("")),
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
	//	Attach dialog items
	//
	
	HWND hwndProfiles = GetDlgItem(IDC_PROFILELIST);
	m_cboProfiles.Attach(hwndProfiles);

	//
	//	Init paths
	//

	TCHAR szBuf[MAX_PATH];
	GetModuleFileName(NULL, szBuf, MAX_PATH); // Get the executable path

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
	if (m_hTitleFont)
	{
		DeleteObject(m_hTitleFont);
	}

	return 0;
}

LRESULT CMainDlg::OnStart(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	/////////////////////////////////////////////////////////////////////////
	// TODO: Perform test -- pointer values are for Skyrim!
	static bool bRunning = false;

	if (!bRunning)
	{
		// Init tracker parameters
		TrkParam trkParam;

		trkParam.bDisableRoll = true;

		// Init mem tracker parameters
		TrkMemParam trkMemParam;

		trkMemParam.strProcess = _T("TESV.EXE");
		trkMemParam.strModule = _T("TESV.EXE");	// base_addr

		trkMemParam.lstYaw.push_back(0x01739ac4);	// base_addr + offset
		trkMemParam.lstYaw.push_back(0x30);		// (*prev_addr) + offset

		trkMemParam.lstPitch.push_back(0x01739ac4);
		trkMemParam.lstPitch.push_back(0x28);

		trkMemParam.lstRoll.push_back(0x01739ac4);
		trkMemParam.lstRoll.push_back(0x32);

		// Init transformation values
		TrkTransform trkTransform;

		trkTransform.fYawMult = 1.0;
		trkTransform.fYawOffset = 180.0;

		trkTransform.fPitchMult = -1.0;
		trkTransform.fPitchOffset = 90.0;

		// Create once... todo: properly create and destroy when start/stop
		m_pTracker = new CMemTrkDriver(m_nTrackerPort, trkParam, trkMemParam, trkTransform);
		m_pTrackThread = new CThread(m_pTracker);

		// Start
		m_pTrackThread->Start();

		bRunning = true;
	}
	// TODO: End test
	/////////////////////////////////////////////////////////////////////////

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

LRESULT CMainDlg::OnAddProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//
	//	Show profile configuration dialog
	//

	CManageDlg dlg;

	// Show dialog
	int nRet = dlg.DoModal();

	//
	//	Update profile list
	//

	RefreshProfiles();

	//
	//	Select profile
	//

	m_cboProfiles.SelectString(-1, dlg.m_strProfileName);

	return 0;
}

LRESULT CMainDlg::OnDeleteProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	// TODO: Delete profile

	//
	//	Update profile list
	//

	RefreshProfiles();

	//
	//	Select first profile
	//

	m_cboProfiles.SetCurSel(0);

	return 0;
}

LRESULT CMainDlg::OnConfigProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	int nCurSel = m_cboProfiles.GetCurSel();

	if (nCurSel < 0)
	{
		ATLASSERT(0 <= nCurSel);
		return 0;
	}
	
	//
	//	Get profile path
	//

	// Get profile path index
	int nIndex = m_cboProfiles.GetItemData(nCurSel);
	
	if ((nIndex < 0) || ((int)m_lstProfilePaths.size() <= nIndex))
	{
		ATLASSERT((0 <= nIndex) && (nIndex < (int)m_lstProfilePaths.size()));
		return 0;
	}

	CString strPath = m_lstProfilePaths[nIndex];

	//
	//	Show profile configuration dialog
	//

	CManageDlg dlg(strPath);

	// Show dialog
	int nRet = dlg.DoModal();

	//
	//	Update profile list
	//

	RefreshProfiles();

	//
	//	Select profile
	//

	m_cboProfiles.SelectString(-1, dlg.m_strProfileName);

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
	// Clear profile list
	m_cboProfiles.ResetContent();
	
	// Clear profile path list
	m_lstProfilePaths.clear();

	// Clear last error
	SetLastError(0);

	// Make search path
	CString strPath = m_strProfilePath;

	if (strPath[strPath.GetLength() - 1] != '\\')
	{
		strPath.Append(_T("\\"));
	}

	strPath.Append(_T("*"));
	strPath.Append(PROFILE_EXT);

	// File entry for enumeration
	WIN32_FIND_DATA fdCurFile;
	ZeroMemory(&fdCurFile, sizeof(fdCurFile));

	// Find the first file in the directory
	HANDLE hFind = FindFirstFile(strPath, &fdCurFile);

	if (INVALID_HANDLE_VALUE == hFind)
	{
		ATLASSERT(INVALID_HANDLE_VALUE != hFind);
		return;
	}

	// Enumerate files
	do
	{
		if (fdCurFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			// Skip if directory
			continue;
		}

		CString strName = GetProfileName(fdCurFile.cFileName);

		if (strName.GetLength())
		{
			// Add path to profile path list
			int nListPos = m_lstProfilePaths.size();
			m_lstProfilePaths.push_back(fdCurFile.cFileName);
			
			// Add profile to combobox
			int index = m_cboProfiles.AddString(strName);
			m_cboProfiles.SetItemData(index, nListPos);
		}
	}
	while (FindNextFile(hFind, &fdCurFile));

	m_cboProfiles.SetCurSel(0);

	ATLASSERT(!GetLastError());
	FindClose(hFind);
}

CString CMainDlg::GetProfileName(CString strPath)
{
	CString strName = _T("");
	TCHAR szBuf[MAX_PATH];

	DWORD nRet = GetPrivateProfileString(_T("Profile"), _T("name"), NULL, szBuf, MAX_PATH, strPath);

	if (nRet)
	{
		strName = szBuf;
	}

	return strName;
}

HRESULT CMainDlg::LoadConfig(CString strPath)
{
	TCHAR szBuf[MAX_PATH]; // Temporary buffer for reading
	DWORD nRet = 0;

	// Clear last error
	SetLastError(0);

	//
	//	Application settings
	//

	// Profile path
	nRet = GetPrivateProfileString(_T("Application"), _T("profile_path"), NULL, szBuf, MAX_PATH, strPath);

	if (nRet)
	{
		m_strProfilePath = szBuf;
	}

	//
	//	Device settings
	//

	// Tracker port
	nRet = GetPrivateProfileString(_T("Device"), _T("tracker_port"), NULL, szBuf, MAX_PATH, strPath);

	if (nRet)
	{
		int nTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 < nTemp))
		{
			m_nTrackerPort = nTemp;
		}
	}

	// Display port
	nRet = GetPrivateProfileString(_T("Device"), _T("display_port"), NULL, szBuf, MAX_PATH, strPath);

	if (nRet)
	{
		int nTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 < nTemp))
		{
			m_nDisplayPort = nTemp;
		}
	}

	// Compression level
	nRet = GetPrivateProfileString(_T("Device"), _T("compression"), NULL, szBuf, MAX_PATH, strPath);

	if (nRet)
	{
		int nTemp = _ttoi(szBuf);
		if (!GetLastError() && (0 <= nTemp))
		{
			m_nCompressionLvl = nTemp;
		}
	}

	return S_OK;
}

HRESULT CMainDlg::SaveConfig(CString strPath)
{
	TCHAR szBuf[MAX_PATH]; // Temporary buffer for writing
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
	_stprintf_s(szBuf, MAX_PATH, _T("%u"), m_nTrackerPort);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("tracker_port"), szBuf, strPath);
	ATLASSERT(bSuccess);

	// Display port
	_stprintf_s(szBuf, MAX_PATH, _T("%u"), m_nDisplayPort);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("display_port"), szBuf, strPath);
	ATLASSERT(bSuccess);

	// Compression level
	_stprintf_s(szBuf, MAX_PATH, _T("%u"), m_nCompressionLvl);
	bSuccess = WritePrivateProfileString(_T("Device"), _T("compression"), szBuf, strPath);
	ATLASSERT(bSuccess);

	return S_OK;
}
