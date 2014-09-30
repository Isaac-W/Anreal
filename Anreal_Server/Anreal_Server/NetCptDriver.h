#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <WinSock2.h>
#include <Windows.h>

#include "ThreadHelper.h"
#include "DxCapture.h"

// Class that drives the screen capture + transmit process
// TODO - Utilize multiple capture methods;
// use either passed pointers (calling function must create/destroy capture) or templates.
class CNetCptDriver :
	public CRunnable
{
public:
	// Default constructor.
	CNetCptDriver(HWND hWnd, const CptParam &cptParam);

	// Destructor.
	virtual ~CNetCptDriver();

	// Start the screen transmit process.
	virtual void Run();

private:
	CCapture *m_pCapture;				// Pointer to member screen capture

	CptParam m_cptParam;				// Standard capture parameters
};

