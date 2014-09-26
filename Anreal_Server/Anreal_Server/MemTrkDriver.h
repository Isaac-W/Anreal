#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <list>
#include <Windows.h>
#include <TlHelp32.h>

#include "ThreadHelper.h"
#include "NetTracker.h"

/////////////////////////////////////////////////////////////////////////////
// Memory tracker helpers

typedef std::list<int> OFFSETLIST; // List containing pointer offsets

// Struct containing parameters for the memory tracker.
// Describes the memory locations of the camera angles, and the storage format (rad/deg).
struct TrkMemParam
{
	TrkMemParam() :
		bYawToDeg(false),
		bPitchToDeg(false),
		bRollToDeg(false),
		strProcess(_T("")),
		strModule(_T(""))
	{
	}

	bool bYawToDeg;			// Yaw angle needs conversion to degrees
	bool bPitchToDeg;		// Pitch angle needs conversion to degrees
	bool bRollToDeg;		// Roll angle needs conversion to degrees

	CString strProcess;		// Name of process
	CString strModule;		// Name of base address module

	OFFSETLIST lstYaw;		// List of pointer offsets for yaw
	OFFSETLIST lstPitch;	// List of pointer offsets for pitch
	OFFSETLIST lstRoll;		// List of pointer offsets for roll
};

/////////////////////////////////////////////////////////////////////////////
// CMemTrkDriver

// Class that uses memory injection to provide in-game head tracking.
// The application must have sufficient admin/debug privileges in order to use.
class CMemTrkDriver :
	public CRunnable
{
public:
	// Constructor. Initializes portnum and the pointer offset lists.
	CMemTrkDriver(USHORT nPortNum, const TrkParam &trkParam, const TrkMemParam &trkMemParam);

	// Constructor. Initializes portnum and the specified orientation transformations.
	CMemTrkDriver(USHORT nPortNum, const TrkParam &trkParam, const TrkMemParam &trkMemParam, const TrkTransform &trkTransform);

	// Destructor.
	virtual ~CMemTrkDriver();

	// Gets the transformation values.
	HRESULT GetTransformation(TrkTransform *pTrkTransform);

	// Sets the transformation values.
	HRESULT SetTransformation(const TrkTransform &trkTransform);

	// Starts the tracker process.
	virtual void Run();

private:
	// Loops through offsets and dereferences each pointer level to get the referenced address.
	HRESULT GetOrientationPointer(HANDLE hProcess, const OFFSETLIST &lstOffset, DWORD *pAddress);

private:
	CTracker *m_pTracker;				// Pointer to member tracker
	TrkTransform m_trkTransform;		// Tracker transformation values

	TrkParam m_trkParam;				// Standard tracker parameters
	TrkMemParam m_trkMemParam;			// Mem tracker parameters
};

