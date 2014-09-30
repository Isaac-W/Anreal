#include "stdafx.h"

#include "NetCptDriver.h"

CNetCptDriver::CNetCptDriver(HWND hWnd, const CptParam &cptParam)
{
	// Create capturer
	m_pCapture = new CDxCapture(hWnd, cptParam);
}

CNetCptDriver::~CNetCptDriver()
{
	// Delete capturer
	if (m_pCapture)
	{
		delete m_pCapture;
		m_pCapture = NULL;
	}
}

void CNetCptDriver::Run()
{
	// TODO -- Get capture, compress, transmit, loop
}