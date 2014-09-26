#pragma once

#include "Capture.h"

#include "Tracker.h"
#include "MemTrkDriver.h"

class CProfile
{
public:
	// Default constructor.
	CProfile() :
		m_strName(_T("Profile")),
		m_cptSelection(CT_DIRECTX),
		m_bCaptureRegion(false),
		m_rcCapture(0, 0, 0, 0),
		m_trkSelection(TT_MEMORY)
	{
	}

	// Deconstructor.
	~CProfile()
	{
	}

	// Loads the profile from the specified file.
	void Load(CString strPath)
	{
		TCHAR szBuf[_MAX_PATH]; // Temporary buffer for reading
		DWORD nRet = 0;

		// Clear last error
		GetLastError();

		//
		//	Profile
		//

		// Name
		nRet = GetPrivateProfileString(_T("Profile"), _T("name"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			m_strName = szBuf;
		}

		//
		//	Capture
		//

		// Capture type
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_type"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp) && (nTemp < CT_COUNT))
			{
				m_cptSelection = (CaptureType)(nTemp);
			}
		}

		// Capture region selection
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_region"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_bCaptureRegion = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		//
		//	Capture area
		//

		// Capture area X
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_x"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp))
			{
				m_rcCapture.left = nTemp;
			}
		}

		// Capture area Y
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_y"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp))
			{
				m_rcCapture.top = nTemp;
			}
		}

		// Capture area width
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_width"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp))
			{
				m_rcCapture.right = m_rcCapture.left + nTemp;
			}
		}

		// Capture area height
		nRet = GetPrivateProfileString(_T("Capture"), _T("capture_height"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp))
			{
				m_rcCapture.bottom = m_rcCapture.top + nTemp;
			}
		}

		//
		//	Tracker
		//

		// Tracker type
		nRet = GetPrivateProfileString(_T("Tracker"), _T("tracker_type"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			int nTemp = _ttoi(szBuf);
			if (!GetLastError() && (0 <= nTemp) && (nTemp < TT_COUNT))
			{
				m_trkSelection = (TrackerType)(nTemp);
			}
		}

		//
		//	TrkParam
		//

		// Disable yaw
		nRet = GetPrivateProfileString(_T("Tracker"), _T("disable_yaw"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkParam.bDisableYaw = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		// Disable pitch
		nRet = GetPrivateProfileString(_T("Tracker"), _T("disable_pitch"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkParam.bDisablePitch = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		// Disable roll
		nRet = GetPrivateProfileString(_T("Tracker"), _T("disable_roll"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkParam.bDisableRoll = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		//
		//	TrkTransform
		//

		// Yaw mult
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("yaw_mult"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fYawMult = (float)(dblTemp);
			}
		}

		// Pitch mult
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("pitch_mult"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fPitchMult = (float)(dblTemp);
			}
		}

		// Roll mult
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("roll_mult"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fRollMult = (float)(dblTemp);
			}
		}

		// Yaw offset
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("yaw_offset"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fYawOffset = (float)(dblTemp);
			}
		}

		// Pitch offset
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("pitch_offset"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fPitchOffset = (float)(dblTemp);
			}
		}

		// Roll offset
		nRet = GetPrivateProfileString(_T("TrackerTransform"), _T("roll_offset"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			double dblTemp = _ttof(szBuf);
			if (!GetLastError())
			{
				m_trkTransform.fRollOffset = (float)(dblTemp);
			}
		}

		//
		//	TrkMemParam
		//

		// Yaw to degrees
		nRet = GetPrivateProfileString(_T("MemTracker"), _T("yaw_to_deg"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkMemParam.bYawToDeg = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		// Pitch to degrees
		nRet = GetPrivateProfileString(_T("MemTracker"), _T("pitch_to_deg"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkMemParam.bPitchToDeg = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		// Roll to degrees
		nRet = GetPrivateProfileString(_T("MemTracker"), _T("roll_to_deg"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;
			m_trkMemParam.bRollToDeg = strTemp.CompareNoCase(_T("true")) ? true : false;
		}

		// Process name
		nRet = GetPrivateProfileString(_T("MemTracker"), _T("process_name"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			m_trkMemParam.strProcess = szBuf;
		}

		// Module name
		nRet = GetPrivateProfileString(_T("MemTracker"), _T("module_name"), NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			m_trkMemParam.strModule = szBuf;
		}

		//
		//	MemTrackerYaw
		//

		// Yaw tracker pointers
		nRet = GetPrivateProfileString(_T("MemTrackerYaw"), NULL, NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;

			CString strKey;
			int nStart = 0;

			while (nStart >= 0)
			{
				strKey = strTemp.Tokenize(_T("\0\n\t "), nStart);

				nRet = GetPrivateProfileString(_T("MemTrackerYaw"), strKey, NULL, szBuf, _MAX_PATH, strPath);

				if (nRet)
				{
					int nTemp = _ttoi(szBuf);
					if (!GetLastError())
					{
						m_trkMemParam.lstYaw.push_back(nTemp);
					}
				}
			}
		}

		//
		//	MemTrackerPitch
		//

		// Pitch tracker pointers
		nRet = GetPrivateProfileString(_T("MemTrackerPitch"), NULL, NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;

			CString strKey;
			int nStart = 0;

			while (nStart >= 0)
			{
				strKey = strTemp.Tokenize(_T("\0\n\t "), nStart);

				nRet = GetPrivateProfileString(_T("MemTrackerYaw"), strKey, NULL, szBuf, _MAX_PATH, strPath);

				if (nRet)
				{
					int nTemp = _ttoi(szBuf);
					if (!GetLastError())
					{
						m_trkMemParam.lstPitch.push_back(nTemp);
					}
				}
			}
		}

		//
		//	MemTrackerRoll
		//

		// Roll tracker pointers
		nRet = GetPrivateProfileString(_T("MemTrackerRoll"), NULL, NULL, szBuf, _MAX_PATH, strPath);

		if (nRet)
		{
			CString strTemp = szBuf;

			CString strKey;
			int nStart = 0;

			while (nStart >= 0)
			{
				strKey = strTemp.Tokenize(_T("\0\n\t "), nStart);

				nRet = GetPrivateProfileString(_T("MemTrackerYaw"), strKey, NULL, szBuf, _MAX_PATH, strPath);

				if (nRet)
				{
					int nTemp = _ttoi(szBuf);
					if (!GetLastError())
					{
						m_trkMemParam.lstRoll.push_back(nTemp);
					}
				}
			}
		}
	}

	// Saves the profile to the specified file.
	void Save(CString strPath)
	{

	}

public:
	//
	//	Profile
	//

	CString m_strName;

	//
	//	Capture
	//

	enum CaptureType
	{
		CT_DISABLED=0,
		CT_GDI,
		CT_DIRECTX,
		CT_COUNT
	};

	CaptureType m_cptSelection;

	bool m_bCaptureRegion;
	CRect m_rcCapture;

	//
	//	Tracker
	//

	enum TrackerType
	{
		TT_DISABLED=0,
		TT_MOUSE,
		TT_MEMORY,
		TT_SMT,
		TT_COUNT
	};

	TrackerType m_trkSelection;

	TrkParam m_trkParam;
	TrkTransform m_trkTransform;

	TrkMemParam m_trkMemParam;
};

