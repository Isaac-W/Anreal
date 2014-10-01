// ManageDlg.h : interface of the CManageDlg class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Profile.h"

// Dialog to manage a profile configuration. Member profile instance is used for initialization.
// After the dialog is closed, calling function may copy settings from the member profile.
class CManageDlg :
	public CDialogImpl<CManageDlg>
{
public:
	enum { IDD = IDD_MANAGEDLG };

	// Constructor. Initializes a new, blank profile
	// strProfileDir must be an absolute path to the profile directory.
	CManageDlg(CString strProfileDir);

	// Constructor. Loads a profile from the specified file.
	// strProfileDir must be an absolute path to the profile directory.
	// strProfileFile must be an absolute path to the profile file.
	CManageDlg(CString strProfileDir, CString strProfileFile);

	~CManageDlg();

	BEGIN_MSG_MAP(CManageDlg)
		MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	END_MSG_MAP()

private:
	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);
	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/);

	LRESULT OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);
	LRESULT OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/);

public:
	CString m_strProfileName;

private:
	CString m_strProfileDir;
	CString m_strProfileFile;
	CProfile m_prfConfig;
};
