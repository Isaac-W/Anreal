// Anreal_Server.cpp : main source file for Anreal_Server.exe
//

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "ConfigDlg.h"

CAppModule _Module;

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes;

	//
	//	Init ATL/WTL
	//

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	//
	//	Create message loop
	//

	CMessageLoop msgLoop;
	_Module.AddMessageLoop(&msgLoop);

	//
	//	Create main dialog
	//

	CMainDlg dlgMain;
	if(!dlgMain.Create(NULL))
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(nCmdShow);

	//
	//	Start the message pump
	//

	int nRet = msgLoop.Run();

	//
	//	Cleanup
	//

	_Module.RemoveMessageLoop();
	_Module.Term();

	return nRet;
}
