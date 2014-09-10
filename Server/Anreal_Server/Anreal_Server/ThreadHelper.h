// Like Hamburger Helper but with over 9000% more fiber -- all those threads!

#pragma once

// 
#define HTHREAD HANDLE

/////////////////////////////////////////////////////////////////////////////
// CRunnable

// Class that represents a runnable object. Inheriting class must override Run().
class CRunnable
{
public:
	// Default constructor.
	CRunnable() :
		m_bRequestStop(false)
	{
	}

	// A procedure that is intended to be executed by a thread.
	virtual void Run() = 0;
private:
	bool m_bRequestStop; // Flag that may be set to notify the procedure to stop. Use if writing a loop in Run().
};

/////////////////////////////////////////////////////////////////////////////
// CThread

// Class that encapsulates a thread of execution. Extends CRunnable.
// CThread may be extended to provide its own Run() implementation.
class CThread :
	public CRunnable
{
public:
	CThread()
	{
		// TODO
	}

	~CThread()
	{
		if (m_hThread) CloseHandle(m_hThread); // Release handle
	}

	void Start()
	{
		m_hThread = CreateThread(NULL,
								 0,
								 StartRunnable,
								 m_pRunnable,
								 0,
								 &m_dwThreadID);

		// TODO
	}

	// Force stops the thread.
	void Stop()
	{
		// TODO
	}

	// Dummy method.
	virtual void Run()
	{
		// TODO
	}

private:
	// Thread management
	HTHREAD m_hThread;			// Handle to Win32 thread
	DWORD m_dwThreadID;			// Thread ID

	// CRunnable variables
	CRunnable *m_pRunnable;		// Pointer to CRunnable instance
};

// Thread callback. Parameter is a pointer to a CRunnable to execute in the thread.
DWORD WINAPI StartRunnable(LPVOID lpParameter)
{
	CRunnable *pRunnable = (CRunnable *)lpParameter;
	if (pRunnable) pRunnable->Run(); // Safety check
}