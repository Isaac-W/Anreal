#include "stdafx.h"
#include "ThreadHelper.h"

DWORD WINAPI RunnableProc(LPVOID lpParameter)
{
	CRunnable *pRunnable = (CRunnable *)lpParameter;
	if (pRunnable) pRunnable->Run(); // Safety check

	return 0;
}

CThread::CThread()
{
	m_pRunnable = this;
}

CThread::CThread(CRunnable *pVar)
{
	m_pRunnable = pVar;
}

CThread::~CThread()
{
	if (m_hThread) CloseHandle(m_hThread); // Release handle
}

void CThread::Start()
{
	m_hThread = CreateThread(NULL,
							 0,
							 RunnableProc,
							 m_pRunnable,
							 0,
							 &m_dwThreadID);
}

void CThread::Stop()
{
	// TODO
}

void CThread::Run()
{
}