#include "stdafx.h"
#include "MemTDriver.h"

CMemTDriver::CMemTDriver(USHORT nPortNum, const TMemParam &trkParam) :
	m_trkTransform()
{
	// Create new network tracker
	m_pTracker = new CNetTracker(nPortNum);

	// Copy offset lists
	m_trkParam = trkParam;
}

CMemTDriver::CMemTDriver(USHORT nPortNum, const TMemParam &trkParam, const TTransform &trkTransform)
{
	// Create new network tracker
	m_pTracker = new CNetTracker(nPortNum);

	// Copy offset lists
	m_trkParam = trkParam;

	// Copy transformation values
	m_trkTransform = trkTransform;
}

CMemTDriver::~CMemTDriver()
{
	// Delete tracker
	if (m_pTracker)
	{
		delete m_pTracker;
		m_pTracker = NULL;
	}
}

HRESULT CMemTDriver::GetTransformation(TTransform *pTrkTransform)
{
	if (!pTrkTransform)
	{
		return E_POINTER;
	}

	(*pTrkTransform) = m_trkTransform;

	return S_OK;
}

HRESULT CMemTDriver::SetTransformation(const TTransform &trkTransform)
{
	m_trkTransform = trkTransform;

	return S_OK;
}

void CMemTDriver::Run()
{
	//
	//	Get process handle
	//

	// Get snapshot of all processes
	HANDLE hProcSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	
	if (!hProcSnap)
	{
		// Failed to get snapshot of processes
		ATLASSERT(hProcSnap);
		return;
	}

	// Process entry for enumeration
	PROCESSENTRY32 procEntry;
	ZeroMemory(&procEntry, sizeof(procEntry));
	procEntry.dwSize = sizeof(procEntry);

	// Test for first process
	if (!Process32First(hProcSnap, &procEntry))
	{
		// Failed to get entry from process snapshot
		ATLASSERT(ERROR_NO_MORE_FILES != GetLastError());
		CloseHandle(hProcSnap);
		return;
	}

	// Process ID
	DWORD nProcessID = 0;

	// Enumerate processes
	do
	{
		// Compare name
		if (m_trkParam.strProcess.CompareNoCase(procEntry.szExeFile) == 0)
		{
			nProcessID = procEntry.th32ProcessID;
			break;
		}
	}
	while (Process32Next(hProcSnap, &procEntry));

	CloseHandle(hProcSnap);

	if (!nProcessID)
	{
		// A matching process name was not found
		ATLASSERT(nProcessID);
		return;
	}

	// Open process
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, TRUE, nProcessID);

	if (!hProcess)
	{
		ATLASSERT(hProcess);
		return;
	}

	//
	//	Get module base address
	//

	DWORD nBaseAddr = 0;

	// Derive module address from process if name is not blank
	if (m_trkParam.strModule.GetLength())
	{
		// Get snapshot of all processes in process
		HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, nProcessID);

		if (!hModuleSnap)
		{
			// Failed to get snapshot of modules
			ATLASSERT(hModuleSnap);
			return;
		}

		// Module entry for enumeration
		MODULEENTRY32 moduleEntry;
		ZeroMemory(&moduleEntry, sizeof(moduleEntry));
		moduleEntry.dwSize = sizeof(moduleEntry);

		// Test for first process
		if (!Module32First(hModuleSnap, &moduleEntry))
		{
			// Failed to get entry from module snapshot
			ATLASSERT(ERROR_NO_MORE_FILES != GetLastError());
			CloseHandle(hModuleSnap);
			return;
		}

		// Module found flag
		bool bFoundModule = false;

		// Enumerate modules
		do
		{
			if (m_trkParam.strModule.CompareNoCase(moduleEntry.szModule) == 0)
			{
				nBaseAddr = (DWORD)(moduleEntry.modBaseAddr);
				bFoundModule = true;
				break;
			}
		}
		while (Module32Next(hModuleSnap, &moduleEntry));

		CloseHandle(hModuleSnap);

		if (!bFoundModule)
		{
			// A matching module name was not found
			ATLASSERT(bFoundModule);
			return;
		}
	}

	//
	//	Get final orientation pointers
	//

	DWORD nYawAddr = 0;
	DWORD nPitchAddr = 0;
	DWORD nRollAddr = 0;
	
	// Get Yaw pointer
	if (!m_trkParam.bDisableYaw)
	{
		nYawAddr += nBaseAddr;
		GetOrientationPointer(hProcess, m_trkParam.lstYaw, &nYawAddr);
		ATLASSERT(nYawAddr);
	}

	// Get Pitch pointer
	if (!m_trkParam.bDisablePitch)
	{
		nPitchAddr += nBaseAddr;
		GetOrientationPointer(hProcess, m_trkParam.lstPitch, &nPitchAddr);
		ATLASSERT(nPitchAddr);
	}

	// Get Roll pointer
	if (!m_trkParam.bDisableRoll)
	{
		nRollAddr += nBaseAddr;
		GetOrientationPointer(hProcess, m_trkParam.lstRoll, &nRollAddr);
		ATLASSERT(nRollAddr);
	}

	//
	//	Main loop
	//

	HRESULT res;
	TOrientation trkOrientation;
	ULONG nPrevTime = GetTickCount();

	while (!IsStopRequested())
	{
		//
		//	Get orientation
		//

		res = m_pTracker->GetOrientation(&trkOrientation);
		ATLASSERT(SUCCEEDED(res));

		//
		//	Inject orientation values into memory
		//

		// Yaw
		if (!m_trkParam.bDisableYaw)
		{
			// Apply transformation
			float fVal = trkOrientation.fYaw * m_trkTransform.fYawMult + m_trkTransform.fYawOffset;

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nYawAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Yaw value.");
			}
		}

		// Pitch
		if (!m_trkParam.bDisablePitch)
		{
			// Apply transformation
			float fVal = trkOrientation.fPitch * m_trkTransform.fPitchMult + m_trkTransform.fPitchOffset;

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nPitchAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Pitch value.");
			}
		}

		// Roll
		if (!m_trkParam.bDisableRoll)
		{
			// Apply transformation
			float fVal = trkOrientation.fRoll * m_trkTransform.fRollMult + m_trkTransform.fRollOffset;

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nRollAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Roll value.");
			}
		}

		//
		//	Sleep execution
		//

		ULONG nCurTime = GetTickCount();
		ULONG nElapsed = nCurTime - nPrevTime;

		if (nElapsed < TRK_SLEEP_TIME)
		{
			Sleep(TRK_SLEEP_TIME - nElapsed);
			nElapsed += TRK_SLEEP_TIME;
		}
		else
		{
			// We are lagging behind; don't sleep thread
			nPrevTime = nCurTime;
		}
	}
}

HRESULT CMemTDriver::GetOrientationPointer(HANDLE hProcess, const OFFSETLIST &lstOffset, DWORD *pAddress)
{
	DWORD nAddress = (*pAddress);

	if (lstOffset.size())
	{
		// Get first offset
		OFFSETLIST::const_iterator it = lstOffset.cbegin();
		nAddress += (*it);
		++it;

		// Loop through offsets and dereference each pointer level
		while (it != lstOffset.cend())
		{
			DWORD nNextAddr = 0;

			// Read memory to get new pointer address
			if (0 == ReadProcessMemory(hProcess, (LPVOID)(nAddress), &nNextAddr, sizeof(nNextAddr), NULL))
			{
				ATLASSERT(nNextAddr);
				return E_FAIL;
			}

			// Add offset to new address
			nAddress = nNextAddr + (*it);

			++it;
		}
	}

	(*pAddress) = nAddress;

	return S_OK;
}
