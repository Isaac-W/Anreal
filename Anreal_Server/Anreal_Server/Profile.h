#pragma once

#include "Capture.h"

#include "Tracker.h"
#include "MemTrkDriver.h"

class CProfile
{
public:
	// Default constructor.
	CProfile() :
		m_strName(_T("")),
		m_cptSelection(CT_DIRECTX),
		m_bCaptureArea(false),
		m_rcCapture(0, 0, 0, 0),
		m_trkSelection(TT_MEMORY),
		m_bDisableYaw(false),
		m_bDisablePitch(false),
		m_bDisableRoll(false)
	{
	}

	// Deconstructor.
	~CProfile()
	{
	}

	// Loads the profile from the specified file.
	void Load(CString strFileName);

	// Saves the profile to the specified file.
	void Save(CString strFileName);

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

	bool m_bCaptureArea;
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

	bool m_bDisableYaw;
	bool m_bDisablePitch;
	bool m_bDisableRoll;

	TrkTransform m_trkTransform;
	TrkMemParam m_trkMemParam;
};

