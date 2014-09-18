// Like Hamburger Helper but with over 9000% more fiber!

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

/////////////////////////////////////////////////////////////////////////////
// Function prototypes

// Thread callback. Parameter is a pointer to a CRunnable to execute in the thread.
DWORD WINAPI RunnableProc(LPVOID pParameter);

/////////////////////////////////////////////////////////////////////////////
// CRunnable

// Class that represents a runnable object. Inheriting class must override Run().
class CRunnable
{
public:
	// Default constructor. Initializes stop request flag.
	CRunnable() :
		m_bRequestStop(false)
	{
		InitializeCriticalSection(&m_csRequestStop);
	}

	// Destructor. Releases critical section resources.
	~CRunnable()
	{
		DeleteCriticalSection(&m_csRequestStop);
	}

	// Returns true if the stop flag has been set.
	bool IsStopRequested()
	{
		bool bTemp = false;
		
		EnterCriticalSection(&m_csRequestStop);
		{
			bTemp = m_bRequestStop;
		}
		LeaveCriticalSection(&m_csRequestStop);

		return bTemp;
	}

	// Raises the m_bRequestStop flag.
	void RequestStop()
	{
		EnterCriticalSection(&m_csRequestStop);
		{
			m_bRequestStop = true;
		}
		LeaveCriticalSection(&m_csRequestStop);
	}

	// A procedure that is intended to be executed by a thread.
	virtual void Run() = 0;

private:
	bool m_bRequestStop; // Flag that may be set to notify the procedure to stop. Use if writing a loop in Run().
	CRITICAL_SECTION m_csRequestStop; // Critical section for the m_bRequestStop variable
};

/////////////////////////////////////////////////////////////////////////////
// CThread

// Class that encapsulates a thread of execution. Inherits from CRunnable.
// CThread may be extended to provide a local Run() implementation.
class CThread :
	public CRunnable
{
public:
	// Default constructor. Sets CRunnable pointer to the current instance.
	CThread();
	
	// Overloaded constructor. Sets CRunnable pointer to the passed argument.
	CThread(CRunnable *pVar);

	// Destructor. Performs cleanup of class variables.
	~CThread();

	// Starts execution of the thread.
	void Start();

	// Force stops the thread.
	void Stop();

	// Dummy method to allow creation of CThread instances.
	// Method may be overridden to provide a local Run() implementation.
	virtual void Run();

private:
	// Thread management
	HANDLE m_hThread;			// Handle to Win32 thread
	DWORD m_nThreadID;			// Thread ID

	// Runnable
	CRunnable *m_pRunnable;		// Pointer to CRunnable instance
};
