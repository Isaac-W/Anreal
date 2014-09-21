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

typedef std::list<DWORD> OFFSETLIST;

struct TMemParam
{
	TMemParam() :
		strProcess(_T("")),
		strModule(_T("")),
		bDisableYaw(false),
		bDisablePitch(false),
		bDisableRoll(false)
	{
	}

	CString strProcess;		// Name of process
	CString strModule;		// Name of base address module

	bool bDisableYaw;		
	bool bDisablePitch;		
	bool bDisableRoll;		

	OFFSETLIST lstYaw;		// List of pointer offsets for yaw
	OFFSETLIST lstPitch;	// List of pointer offsets for pitch
	OFFSETLIST lstRoll;		// List of pointer offsets for roll
};

/////////////////////////////////////////////////////////////////////////////
// CMemTDriver

// Class that uses memory injection to provide in-game head tracking.
// The application must have sufficient admin/debug privileges in order to use.
class CMemTDriver :
	public CRunnable
{
public:
	// Constructor. Initializes portnum and the pointer offset lists.
	CMemTDriver(USHORT nPortNum, const TMemParam &trkParam);

	// Constructor. Initializes portnum and the specified orientation transformations.
	CMemTDriver(USHORT nPortNum, const TMemParam &trkParam, const TTransform &trkTransform);

	// Destructor.
	virtual ~CMemTDriver();

	// Gets the transformation values.
	HRESULT GetTransformation(TTransform *pTrkTransform);

	// Sets the transformation values.
	HRESULT SetTransformation(const TTransform &trkTransform);

	// Starts the tracker process.
	virtual void Run();

private:
	// Loops through offsets and dereferences each pointer level to get a new address.
	HRESULT GetOrientationPointer(HANDLE hProcess, const OFFSETLIST &lstOffset, DWORD *pAddress);

private:
	CTracker *m_pTracker;			// Pointer to member tracker
	TTransform m_trkTransform;		// Tracker transformation values

	TMemParam m_trkParam;			// Mem tracker parameters
};

