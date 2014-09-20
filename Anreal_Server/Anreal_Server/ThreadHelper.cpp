#include "stdafx.h"
#include "ThreadHelper.h"

DWORD WINAPI RunnableProc(LPVOID pParameter)
{
	CRunnable *pRunnable = (CRunnable *)pParameter;
	
	if (pRunnable)
	{
		pRunnable->Run();
	}

	return 0;
}

CThread::CThread()
{
	m_pRunnable = this; // Set runnable to self
}

CThread::CThread(CRunnable *pVar)
{
	m_pRunnable = pVar;
}

CThread::~CThread()
{
	// Force stop/cleanup of thread
	Stop();
}

void CThread::Start()
{
	m_hThread = CreateThread(NULL,
							 0,
							 RunnableProc,
							 m_pRunnable,
							 0,
							 &m_nThreadID);

	ATLASSERT(m_hThread);
}

void CThread::Stop()
{
	if (!m_hThread)
	{
		return; // Nothing to do!
	}

	// Raise force stop flag
	m_pRunnable->RequestStop();

	// Wait for thread to finish execution (blocks)
	DWORD nWaitRet = WaitForSingleObject(m_hThread, INFINITE);
	ATLASSERT(0 == nWaitRet);

	// Get exit code
	DWORD nExitCode = GetExitCodeThread(m_hThread, &nExitCode);
	ATLASSERT(STILL_ACTIVE != nExitCode);

	// Release thread handle
	BOOL bSuccess = CloseHandle(m_hThread);
	ATLASSERT(bSuccess);

	m_hThread = NULL;
}

void CThread::Run()
{
	// Do nothing
}
