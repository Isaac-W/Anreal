#include "stdafx.h"
#include "MemTrkDriver.h"

CMemTrkDriver::CMemTrkDriver(USHORT nPortNum, const TrkMemParam &trkParam) :
	m_trkTransform()
{
	// Create new network tracker
	m_pTracker = new CNetTracker(nPortNum);

	// Copy offset lists
	m_trkParam = trkParam;
}

CMemTrkDriver::CMemTrkDriver(USHORT nPortNum, const TrkMemParam &trkParam, const TrkTransform &trkTransform)
{
	// Create new network tracker
	m_pTracker = new CNetTracker(nPortNum);

	// Copy offset lists
	m_trkParam = trkParam;

	// Copy transformation values
	m_trkTransform = trkTransform;
}

CMemTrkDriver::~CMemTrkDriver()
{
	// Delete tracker
	if (m_pTracker)
	{
		delete m_pTracker;
		m_pTracker = NULL;
	}
}

HRESULT CMemTrkDriver::GetTransformation(TrkTransform *pTrkTransform)
{
	if (!pTrkTransform)
	{
		return E_POINTER;
	}

	(*pTrkTransform) = m_trkTransform;

	return S_OK;
}

HRESULT CMemTrkDriver::SetTransformation(const TrkTransform &trkTransform)
{
	m_trkTransform = trkTransform;

	return S_OK;
}

void CMemTrkDriver::Run()
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

	// Derive module address from process if name is not blank;
	// if module name is blank; base addr is considered 0... use offsets only!
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
	//	Follow offsets to get address of camera angles in memory
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
	//	Main tracker loop
	//

	HRESULT res;
	TrkOrientation trkOrientation;
	ULONG nPrevTime = GetTickCount();

	const ULONG MAX_SLEEP_TIME = TIME_VAL_SEC / TRK_UPDATE_RATE; // Determine sleep time based on desired rate

	// IMPORTANT: Need to check for stop request flag, in case loop needs to be stopped during program execution!
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

			// Convert radians to degrees
			if (m_trkParam.bYawToDeg)
			{
				fVal *= RAD_TO_DEG;
			}

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nYawAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Yaw value.");
				return;
			}
		}

		// Pitch
		if (!m_trkParam.bDisablePitch)
		{
			// Apply transformation
			float fVal = trkOrientation.fPitch * m_trkTransform.fPitchMult + m_trkTransform.fPitchOffset;

			// Convert radians to degrees
			if (m_trkParam.bPitchToDeg)
			{
				fVal *= RAD_TO_DEG;
			}

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nPitchAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Pitch value.");
				return;
			}
		}

		// Roll
		if (!m_trkParam.bDisableRoll)
		{
			// Apply transformation
			float fVal = trkOrientation.fRoll * m_trkTransform.fRollMult + m_trkTransform.fRollOffset;

			// Convert radians to degrees
			if (m_trkParam.bRollToDeg)
			{
				fVal *= RAD_TO_DEG;
			}

			// Write value to memory
			if (0 == WriteProcessMemory(hProcess, (LPVOID)(nRollAddr), &fVal, sizeof(fVal), NULL))
			{
				ATLASSERT(0 && "Failed to write Roll value.");
				return;
			}
		}

		//
		//	Sleep execution
		//

		ULONG nCurTime = GetTickCount();
		ULONG nElapsed = nCurTime - nPrevTime;

		if (nElapsed < MAX_SLEEP_TIME)
		{
			Sleep(MAX_SLEEP_TIME - nElapsed);
			nElapsed += MAX_SLEEP_TIME;
		}
		else
		{
			// We are lagging behind; don't sleep thread
			nPrevTime = nCurTime;
		}
	}
}

HRESULT CMemTrkDriver::GetOrientationPointer(HANDLE hProcess, const OFFSETLIST &lstOffset, DWORD *pAddress)
{
	DWORD nAddress = (*pAddress);

	if (lstOffset.size())
	{
		// Get first offset (this is applied directly to the base addr)
		OFFSETLIST::const_iterator it = lstOffset.cbegin();
		nAddress += (*it);
		++it;

		// Loop through offsets and dereference each pointer level:
		// If an offset exists, dereference the current address and apply offset to the dereferenced value.
		// Offsets of 0 mean that the pointer should be dereferenced without applying an offset to the resulting addr.
		while (it != lstOffset.cend())
		{
			DWORD nNextAddr = 0;

			// Read memory to get the referenced pointer address
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
