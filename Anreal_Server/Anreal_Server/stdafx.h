// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//  are changed infrequently
//

#pragma once

// Change these values to use different versions
#define WINVER		0x0500
#define _WIN32_WINNT	0x0501
#define _WIN32_IE	0x0501
#define _RICHEDIT_VER	0x0500

#include <atlbase.h>
#include <atlapp.h>

extern CAppModule _Module;

#include <atlwin.h>

#include <atlframe.h>
#include <atlctrls.h>
#include <atldlgs.h>
#include <atltypes.h>
#include <atlstr.h>

/////////////////////////////////////////////////////////////////////////////
// Local headers

#include "ThreadHelper.h"

/////////////////////////////////////////////////////////////////////////////
// Constants

#define CONFIG_PATH _T("\\anreal_server.cfg")	// Path to Anreal config file
#define PROFILE_EXT	_T("anr")					// Profile file extension

#define TRK_BUF_SIZE 512						// Tracker receive buffer size

#define TIME_VAL_SEC 1000						// Time value for a second (1000 ms for GetTickCount)
#define TRK_UPDATE_RATE 60						// Desired tracker update/refresh rate in Hz
#define CPT_UPDATE_RATE 60						// Desired capture update/refresh rate in Hz

#define RAD_TO_DEG 57.29578f					// (180 / PI) to convert from radians to degrees

// Defaults
#define DEFAULT_PROFILE_PATH _T("\\Profiles\\")	// Path to Anreal profiles
#define DEFAULT_TRACKER_PORT 5250				// Default head tracker receive port
#define DEFAULT_CAPTURE_PORT 5251				// Default screen capture transmit port
#define DEFAULT_COMPRESS_LVL 50					// Default image compression level


#if defined _M_IX86
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
  #pragma comment(linker, "/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
