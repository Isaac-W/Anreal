// ConfigDlg.cpp : implementation of the CConfigDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "ConfigDlg.h"

CConfigDlg::CConfigDlg()
{
}

CConfigDlg::~CConfigDlg()
{
}

LRESULT CConfigDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
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
	//	Profile path
	//

	{
		BOOL bSuccess = SetDlgItemText(IDC_PROFILE_PATH, m_strProfilePath);
		ATLASSERT(bSuccess);
	}

	//
	//	Tracker port
	//

	{
		BOOL bSuccess = SetDlgItemInt(IDC_TRACKER_PORT, m_nTrackerPort, FALSE);
		ATLASSERT(bSuccess);

		// Limit input to 5 digits
		SendDlgItemMessage(IDC_TRACKER_PORT, EM_LIMITTEXT, 5, 0);
	}

	//
	//	Display port
	//

	{
		BOOL bSuccess = SetDlgItemInt(IDC_CAPTURE_PORT, m_nDisplayPort, FALSE);
		ATLASSERT(bSuccess);

		// Limit input to 5 digits
		SendDlgItemMessage(IDC_CAPTURE_PORT, EM_LIMITTEXT, 5, 0);
	}

	//
	//	Compression level
	//

	{
		BOOL bSuccess = SetDlgItemInt(IDC_COMPRESSION_LVL, m_nCompressionLvl, FALSE);
		ATLASSERT(bSuccess);

		// Limit input to 3 digits
		SendDlgItemMessage(IDC_COMPRESSION_LVL, EM_LIMITTEXT, 3, 0);
	}

	return TRUE;
}

LRESULT CConfigDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return 0;
}

LRESULT CConfigDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	//
	//	Profile path
	//

	{
		BOOL bSuccess = GetDlgItemText(IDC_PROFILE_PATH, m_strProfilePath);
		ATLASSERT(bSuccess);
	}

	//
	//	Tracker port
	//

	{
		BOOL bSuccess = FALSE;
		int nTemp = GetDlgItemInt(IDC_TRACKER_PORT, &bSuccess, FALSE);
		ATLASSERT(bSuccess);

		if ((0 < nTemp) && (nTemp <= 65535))
		{
			m_nTrackerPort = nTemp;
		}
		else
		{
			MessageBox(_T("Tracker port must be a number from 1 to 65535."), _T("Error"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			return 1;
		}
	}

	//
	//	Display port
	//

	{
		BOOL bSuccess = FALSE;
		int nTemp = GetDlgItemInt(IDC_CAPTURE_PORT, &bSuccess, FALSE);
		ATLASSERT(bSuccess);

		if ((0 < nTemp) && (nTemp <= 65535))
		{
			if (m_nTrackerPort != nTemp)
			{
				m_nDisplayPort = nTemp;
			}
			else
			{
				MessageBox(_T("Display port cannot be the same as Tracker port!"), _T("Error"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
				return 1;
			}
		}
		else
		{
			MessageBox(_T("Display port must be a number from 1 to 65535."), _T("Error"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			return 1;
		}
	}

	//
	//	Compression level
	//

	{
		BOOL bSuccess = FALSE;
		int nTemp = GetDlgItemInt(IDC_COMPRESSION_LVL, &bSuccess, FALSE);
		ATLASSERT(bSuccess);

		if ((0 <= nTemp) && (nTemp <= 100))
		{
			m_nCompressionLvl = nTemp;
		}
		else
		{
			MessageBox(_T("Compression level must be a number from 0 to 100."), _T("Error"), MB_OK | MB_ICONERROR | MB_SETFOREGROUND);
			return 1;
		}
	}

	EndDialog(wID);
	return 0;
}

LRESULT CConfigDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	EndDialog(wID);
	return 0;
}
