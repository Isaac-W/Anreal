// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>

class CMainDlg :
	public CDialogImpl<CMainDlg>
{
public:
	enum { IDD = IDD_MAINDLG };

	CMainDlg();
	~CMainDlg();

	BEGIN_MSG_MAP(CMainDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDC_START, OnStart)
		COMMAND_ID_HANDLER(IDC_CONFIG, OnConfig)
		COMMAND_ID_HANDLER(IDC_NEWPROFILE, OnAddProfile)
		COMMAND_ID_HANDLER(IDC_DELETEPROFILE, OnDeleteProfile)
		COMMAND_ID_HANDLER(IDC_MANAGEPROFILE, OnConfigProfile)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnStart(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConfig(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnAddProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnDeleteProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnConfigProfile(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	void CloseDialog(int nVal);
	
	// Re-populates the profile list from the profile path.
	void RefreshProfiles();

	// Reads the profile name from the specified profile file. strPath must be an absolute path.
	CString GetProfileName(CString strPath);

	// Loads the Anreal configuration from file. strPath must be an absolute path.
	HRESULT LoadConfig(CString strPath);

	// Saves the Anreal configuration to file. strPath must be an absolute path.
	HRESULT SaveConfig(CString strPath);

private:
	// Dialog
	HFONT m_hTitleFont;
	CComboBox m_cboProfiles;

	// App path
	CString m_strAppPath;

	// Threads
	CThread *m_pTrackThread;
	CRunnable *m_pTracker;

	CThread *m_pCaptureThread;
	CRunnable *m_pCapture;

	// Config
	CString m_strConfigPath;
	CString m_strProfilePath;
	USHORT m_nTrackerPort;
	USHORT m_nDisplayPort;
	USHORT m_nCompressionLvl;

	// Profile
	std::deque<CString> m_lstProfilePaths;
};
