// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "MemTracker.h"

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
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
		COMMAND_ID_HANDLER(IDC_CONFIG, OnConfig)
		COMMAND_ID_HANDLER(IDC_MANAGE, OnManageProfiles)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

	LRESULT OnConfig(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnManageProfiles(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

private:
	void CloseDialog(int nVal);
	
	void RefreshProfiles();

	HRESULT LoadConfig(CString strPath);
	HRESULT SaveConfig(CString strPath);
	
	HRESULT LoadProfile(CString strPath);

private:
	HFONT m_hTitleFont;
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
};
