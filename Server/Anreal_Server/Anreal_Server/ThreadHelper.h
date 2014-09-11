// Like Hamburger Helper but with over 9000% more fiber -- all those threads!

#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>

// Function prototypes
DWORD WINAPI RunnableProc(LPVOID lpParameter);

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
	}

	// A procedure that is intended to be executed by a thread.
	virtual void Run() = 0;

public:
	bool m_bRequestStop; // Flag that may be set to notify the procedure to stop. Use if writing a loop in Run().
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
	CThread()
	{
		m_pRunnable = this;
	}
	
	// Overloaded constructor. Sets CRunnable pointer to the passed argument.
	CThread(CRunnable *pVar)
	{
		m_pRunnable = pVar;
	}

	// Destructor. Performs cleanup of class variables.
	~CThread()
	{
		if (m_hThread) CloseHandle(m_hThread); // Release handle
	}

	// Starts execution of the thread.
	void Start()
	{
		m_hThread = CreateThread(NULL,
								 0,
								 RunnableProc,
								 m_pRunnable,
								 0,
								 &m_dwThreadID);
	}

	// Force stops the thread.
	void Stop()
	{
		// TODO
	}

	// Dummy method to allow creation of CThread instances.
	// Method may be overridden to provide a local Run() implementation.
	virtual void Run()
	{
	}

private:
	// Thread management
	HANDLE m_hThread;			// Handle to Win32 thread
	DWORD m_dwThreadID;			// Thread ID

	// 
	CRunnable *m_pRunnable;		// Pointer to CRunnable instance
};

// Thread callback. Parameter is a pointer to a CRunnable to execute in the thread.
DWORD WINAPI RunnableProc(LPVOID lpParameter)
{
	CRunnable *pRunnable = (CRunnable *)lpParameter;
	if (pRunnable) pRunnable->Run(); // Safety check

	return 0;
}