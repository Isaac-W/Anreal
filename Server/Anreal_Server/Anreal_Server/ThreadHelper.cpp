#include "stdafx.h"
#include "ThreadHelper.h"

DWORD WINAPI RunnableProc(LPVOID lpParameter)
{
	CRunnable *pRunnable = (CRunnable *)lpParameter;
	if (pRunnable) pRunnable->Run();

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
							 &m_dwThreadID);

	_ASSERTE(m_hThread);
}

void CThread::Stop()
{
	if (!m_hThread) return; // Nothing to do!

	// Raise force stop flag
	m_pRunnable->RequestStop();

	// Wait for thread to finish execution (blocks)
	DWORD dwWaitRet = WaitForSingleObject(m_hThread, INFINITE);
	_ASSERTE(0 == dwWaitRet);

	// Get exit code
	DWORD dwExitCode = GetExitCodeThread(m_hThread, &dwExitCode);
	_ASSERTE(STILL_ACTIVE != dwExitCode);

	// Release thread handle
	BOOL bSuccess = CloseHandle(m_hThread);
	_ASSERTE(bSuccess);

	m_hThread = NULL;
}

void CThread::Run()
{
	// Do nothing
}